#include "Components.h"

#include <string.h>
#include <ctype.h>
#include <vector>
#include <string>

bool lrcpp::Config::setVariables(struct retro_variable const* const variables) {
    size_t totalLength = 0;

    for (size_t i = 0; variables[i].key != nullptr; i++) {
        totalLength += strlen(variables[i].key) + 1;
        totalLength += strlen(variables[i].value) + 1;
    }

    char* const strings = (char*)malloc(totalLength);

    if (strings == nullptr) {
        return false;
    }

    char* ptr = strings;

    std::vector<struct retro_core_option_definition> defs;

    for (size_t i = 0; variables[i].key != nullptr; i++) {
        struct retro_core_option_definition def;
        def.key = variables[i].key;

        char const* option = strchr(variables[i].value, ';');

        if (option == nullptr) {
            return false;
        }

        def.desc = ptr;
        def.info = nullptr;

        memcpy(ptr, variables[i].value, option - variables[i].value);
        ptr += option - variables[i].value;
        *ptr++ = 0;

        size_t j = 0;
        option++;

        while (isspace(*option)) {
            option++;
        }

        if (*option == 0) {
            return false;
        }

        for (;;) {
            char const* const pipe = strchr(option, '|');
            def.values[j].value = ptr;

            if (pipe == nullptr) {
                strcpy(ptr, option);
                ptr += strlen(ptr) + 1;
            }
            else {
                memcpy(ptr, option, pipe - option);
                ptr += pipe - option;
                *ptr++ = 0;
            }

            def.values[j].label = nullptr;

            if (pipe == nullptr) {
                break;
            }

            option = pipe + 1;
            j++;
        }

        def.default_value = def.values[0].value;
        defs.emplace_back(def);
    }

    struct retro_core_option_definition def;

    def.key = nullptr;
    def.desc = nullptr;
    def.info = nullptr;
    def.values[0].value = nullptr;
    def.values[0].label = nullptr;
    def.default_value = nullptr;

    defs.emplace_back(def);

    bool const ok = setCoreOptions(defs.data());
    free(strings);
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
