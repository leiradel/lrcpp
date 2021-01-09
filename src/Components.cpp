#include "Components.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void lrcpp::Logger::debug(char const* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(RETRO_LOG_DEBUG, format, args);
    va_end(args);
}

void lrcpp::Logger::info(char const* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(RETRO_LOG_INFO, format, args);
    va_end(args);
}

void lrcpp::Logger::warn(char const* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(RETRO_LOG_WARN, format, args);
    va_end(args);
}

void lrcpp::Logger::error(char const* format, ...) {
    va_list args;
    va_start(args, format);
    vprintf(RETRO_LOG_ERROR, format, args);
    va_end(args);
}

bool lrcpp::Config::setVariables(struct retro_variable const* const variables) {
    size_t count = 0;
    size_t totalLength = 0;

    for (count = 0; variables[count].key != nullptr; count++) {
        totalLength += strlen(variables[count].key) + 1;
        totalLength += strlen(variables[count].value) + 1;
    }

    retro_core_option_definition* const defs = (retro_core_option_definition*)malloc((count + 1) * sizeof(*defs));
    char* const strings = (char*)malloc(totalLength);

    if (defs == nullptr || strings == nullptr) {
        free(defs);
        free(strings);
        return false;
    }

    char* ptr = strings;

    for (size_t i = 0; i < count; i++) {
        retro_core_option_definition* const def = defs + i;
        def->key = variables[i].key;

        char const* option = strchr(variables[i].value, ';');

        if (option == nullptr) {
            free(defs);
            free(strings);
            return false;
        }

        def->desc = ptr;
        def->info = nullptr;

        memcpy(ptr, variables[i].value, option - variables[i].value);
        ptr += option - variables[i].value;
        *ptr++ = 0;

        size_t j = 0;
        option++;

        while (isspace(*option)) {
            option++;
        }

        if (*option == 0) {
            free(defs);
            free(strings);
            return false;
        }

        for (;;) {
            char const* const pipe = strchr(option, '|');
            def->values[j].value = ptr;

            if (pipe == nullptr) {
                strcpy(ptr, option);
                ptr += strlen(ptr) + 1;
            }
            else {
                memcpy(ptr, option, pipe - option);
                ptr += pipe - option;
                *ptr++ = 0;
            }

            def->values[j].label = nullptr;

            if (pipe == nullptr) {
                break;
            }

            option = pipe + 1;
            j++;
        }

        def->default_value = def->values[0].value;
    }

    defs[count].key = nullptr;
    defs[count].desc = nullptr;
    defs[count].info = nullptr;
    defs[count].values[0].value = nullptr;
    defs[count].values[0].label = nullptr;
    defs[count].default_value = nullptr;

    bool const ok = setCoreOptions(defs);
    free(strings);
    free(defs);
    return ok;
}

bool lrcpp::Config::getCoreOptionsVersion(unsigned* const version) {
    *version = 1;
    return true;
}

static size_t addBitsDown(size_t n) {
    n |= n >>  1;
    n |= n >>  2;
    n |= n >>  4;
    n |= n >>  8;
    n |= n >> 16;

    if (sizeof(size_t) > 4) {
        // double shift to avoid warnings on 32bit
        n |= n >> 16 >> 16;
    }

    return n;
}

static size_t inflate(size_t addr, size_t mask) {
    while (mask)
    {
        size_t tmp = (mask - 1) & ~mask;
        // to put in an 1 bit instead, OR in tmp+1
        addr = ((addr & ~tmp) << 1) | (addr & tmp);
        mask = mask & (mask - 1);
    }

    return addr;
}

static size_t reduce(size_t addr, size_t mask) {
    while (mask) {
        size_t tmp = (mask - 1) & ~mask;
        addr = (addr & tmp) | ((addr >> 1) & ~tmp);
        mask = (mask & (mask - 1)) >> 1;
    }

    return addr;
}

static size_t highestBit(size_t n) {
    n = addBitsDown(n);
    return n ^ (n >> 1);
}

bool lrcpp::Config::preprocessMemoryDescriptors(struct retro_memory_descriptor* descriptors, unsigned count) {
    size_t disconnect_mask;
    size_t top_addr = 1;

    for (unsigned i = 0; i < count; i++) {
        struct retro_memory_descriptor* const desc = descriptors + i;

        if (desc->select != 0) {
            top_addr |= desc->select;
        }
        else {
            top_addr |= desc->start + desc->len - 1;
        }
    }

    top_addr = addBitsDown(top_addr);

    for (unsigned i = 0; i < count; i++) {
        struct retro_memory_descriptor* const desc = descriptors + i;

        if (desc->select == 0) {
            if (desc->len == 0) {
                return false;
            }

            if ((desc->len & (desc->len - 1)) != 0) {
                return false;
            }

            desc->select = top_addr & ~inflate(addBitsDown(desc->len - 1), desc->disconnect);
        }

        if (desc->len == 0) {
            desc->len = addBitsDown(reduce(top_addr & ~desc->select, desc->disconnect)) + 1;
        }

        if (desc->start & ~desc->select) {
            return false;
        }

        while (reduce(top_addr & ~desc->select, desc->disconnect) >> 1 > desc->len - 1) {
            desc->disconnect |= highestBit(top_addr & ~desc->select & ~desc->disconnect);
        }

        disconnect_mask = addBitsDown(desc->len - 1);
        desc->disconnect &= disconnect_mask;

        while ((~disconnect_mask) >> 1 & desc->disconnect) {
            disconnect_mask >>= 1;
            desc->disconnect &= disconnect_mask;
        }
    }

    return true;
}
