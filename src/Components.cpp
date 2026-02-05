#include <lrcpp/Components.h>

#include <string.h>
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

bool lrcpp::Config::getCoreOptionsVersion(unsigned* const version) {
    *version = 2;
    return true;
}

bool lrcpp::Config::setVariables(struct retro_variable const* const variables) {
    bool ok = false;
    size_t count = 0;
    size_t totalLength = 0;
    char* strings = nullptr;

    for (count = 0; variables[count].key != nullptr; count++) {
        totalLength += strlen(variables[count].key) + 1;
        totalLength += strlen(variables[count].value) + 1;
    }

    auto const us = (retro_core_option_v2_definition*)malloc((count + 1) * sizeof(retro_core_option_v2_definition));
    strings = (char*)malloc(totalLength);

    memset(us, 0, (count + 1) * sizeof(retro_core_option_v2_definition));
    retro_core_options_v2 usopts = {};
    usopts.categories = nullptr;
    usopts.definitions = us;

    retro_core_options_v2_intl intlopts = {};
    intlopts.local = nullptr;
    intlopts.us = &usopts;

    char* ptr = strings;

    if (us == nullptr || strings == nullptr) {
        goto end;
    }

    for (size_t i = 0; i < count; i++) {
        retro_core_option_v2_definition* const def = us + i;
        def->key = variables[i].key;

        char const* option = strchr(variables[i].value, ';');

        if (option == nullptr) {
            goto end;
        }

        def->desc = ptr;

        memcpy(ptr, variables[i].value, option - variables[i].value);
        ptr += option - variables[i].value;
        *ptr++ = 0;

        option++;

        while (*option == ' ' || *option == '\t') {
            option++;
        }

        if (*option == 0) {
            goto end;
        }

        size_t j = 0;

        for (; j < RETRO_NUM_CORE_OPTION_VALUES_MAX - 1; j++) {
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

            if (pipe == nullptr) {
                break;
            }

            option = pipe + 1;
        }

        if (j == RETRO_NUM_CORE_OPTION_VALUES_MAX - 1) {
            goto end;
        }

        def->default_value = def->values[0].value;
    }

    ok = setCoreOptionsV2Intl(&intlopts);

end:
    free(strings);
    free(us);
    return ok;
}

bool lrcpp::Config::getLanguage(unsigned* language) {
    *language = RETRO_LANGUAGE_ENGLISH;
    return true;
}

bool lrcpp::Config::setCoreOptions(retro_core_option_definition const* options) {
    retro_core_option_v2_definition* const us = coreOptionV1ToV2(options);

    if (us == nullptr) {
        return false;
    }

    retro_core_options_v2 usopts = {};
    usopts.categories = nullptr;
    usopts.definitions = us;

    retro_core_options_v2_intl intlopts = {};
    intlopts.us = &usopts;
    intlopts.local = nullptr;
    
    bool const ok = setCoreOptionsV2Intl(&intlopts);

    free(us);
    return ok;
}

bool lrcpp::Config::setCoreOptionsIntl(retro_core_options_intl const* intl) {
    retro_core_option_v2_definition* const us = coreOptionV1ToV2(intl->us);
    retro_core_option_v2_definition* const defs = coreOptionV1ToV2(intl->local);

    if (us == nullptr || (defs == nullptr && intl->local != nullptr)) {
        free(defs);
        free(us);
        return false;
    }

    retro_core_options_v2 usopts = {};
    usopts.categories = nullptr;
    usopts.definitions = us;

    retro_core_options_v2 localopts = {};
    localopts.categories = nullptr;
    localopts.definitions = defs;

    retro_core_options_v2_intl intlopts = {};
    intlopts.us = &usopts;
    intlopts.local = &localopts;

    bool const ok = setCoreOptionsV2Intl(&intlopts);

    free(defs);
    free(us);
    return ok;
}

retro_core_option_v2_definition* lrcpp::Config::coreOptionV1ToV2(retro_core_option_definition const* options) {
    if (options == nullptr) {
        return nullptr;
    }

    bool ok = false;
    size_t count = 0;
    size_t totalLength = 0;

    for (count = 0; options[count].key != nullptr; count++) {
        // just count
    }

    auto const defs = (retro_core_option_v2_definition*)malloc((count + 1) * sizeof(retro_core_option_v2_definition));

    if (defs == nullptr) {
        return nullptr;
    }

    memset(defs, 0, (count + 1) * sizeof(retro_core_option_v2_definition));

    for (size_t i = 0; i < count; i++) {
        retro_core_option_v2_definition* const def = defs + i;
        def->key = options[i].key;

        for (size_t j = 0; j < RETRO_NUM_CORE_OPTION_VALUES_MAX && options[i].values[j].value != nullptr; j++) {
            def->values[j].value = options[i].values[j].value;
        }

        def->default_value = def->values[0].value;
    }

    return defs;
}

size_t lrcpp::Config::addBitsDown(size_t n) {
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

size_t lrcpp::Config::inflate(size_t addr, size_t mask) {
    while (mask)
    {
        size_t tmp = (mask - 1) & ~mask;
        // to put in an 1 bit instead, OR in tmp+1
        addr = ((addr & ~tmp) << 1) | (addr & tmp);
        mask = mask & (mask - 1);
    }

    return addr;
}

size_t lrcpp::Config::reduce(size_t addr, size_t mask) {
    while (mask) {
        size_t tmp = (mask - 1) & ~mask;
        addr = (addr & tmp) | ((addr >> 1) & ~tmp);
        mask = (mask & (mask - 1)) >> 1;
    }

    return addr;
}

size_t lrcpp::Config::highestBit(size_t n) {
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

void lrcpp::Audio::sample(int16_t left, int16_t right) {
    int16_t batch[2] = { left, right };
    sampleBatch(batch, 1);
}
