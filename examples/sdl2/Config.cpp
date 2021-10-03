#include "Config.h"

#include <string.h>

#ifdef WIN32
    #include "realpath.c"
    #define PATH_MAX _MAX_PATH
#endif

Config::Config() {
    reset();
}

bool Config::init(std::string const& iniPath, std::string const& rootDir, std::string const& corePath, lrcpp::Logger* logger) {
    reset();

    _logger = logger;

    if (!initOptions(iniPath)) {
        return false;
    }

    _systemDir = rootDir + "/libretro/system/";

    if (!initCoreDir(corePath)) {
        return false;
    }

    _assetsDir = rootDir + "/libretro/assets/";
    _savesDir = rootDir + "/libretro/saves/";

    return true;
}

void Config::destroy() {
    reset();
}

bool Config::setPerformanceLevel(unsigned level) {
    (void)level;
    _logger->warn("RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL not implemented");
    return false;
}

bool Config::getSystemDirectory(char const** directory) {
    *directory = _systemDir.c_str();
    return true;
}

bool Config::getVariable(retro_variable* variable) {
    auto const found = _options.find(variable->key);

    if (found != _options.end()) {
        variable->value = found->second.c_str();
        _logger->info("Found value \"%s\" for variable \"%s\"", variable->value, variable->key);
        return true;
    }

    variable->value = nullptr;
    _logger->error("Variable \"%s\" not found", variable->key);
    return false;
}

bool Config::getVariableUpdate(bool* const updated) {
    *updated = _optionsUpdated;
    _optionsUpdated = false;
    return true;
}

bool Config::setSupportNoGame(bool const supports) {
    _supportsNoGame = supports;
    return true;
}

bool Config::getLibretroPath(char const** path) {
    *path = _coreDir.c_str();
    return true;
}

bool Config::getCoreAssetsDirectory(char const** directory) {
    *directory = _assetsDir.c_str();
    return true;
}

bool Config::getSaveDirectory(char const** directory) {
    *directory = _savesDir.c_str();
    return true;
}

bool Config::setProcAddressCallback(retro_get_proc_address_interface const* callback) {
    (void)callback;
    _logger->warn("RETRO_ENVIRONMENT_SET_PROC_ADDRESS_CALLBACK not implemented");
    return false;
}

bool Config::setSubsystemInfo(retro_subsystem_info const* info) {
    (void)info;
    _logger->warn("RETRO_ENVIRONMENT_SET_SUBSYSTEM_INFO not implemented");
    return false;
}

bool Config::setMemoryMaps(retro_memory_map const* map) {
    (void)map;
    _logger->warn("RETRO_ENVIRONMENT_SET_MEMORY_MAPS not implemented");
    return false;
}

bool Config::getUsername(char const** username) {
    *username = nullptr;
    return true;
}

bool Config::getLanguage(unsigned* language) {
    *language = RETRO_LANGUAGE_ENGLISH;
    return true;
}

bool Config::setSupportAchievements(bool supports) {
    (void)supports;
    _logger->warn("RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS not implemented");
    return false;
}

bool Config::setSerializationQuirks(uint64_t quirks) {
    (void)quirks;
    _logger->warn("RETRO_ENVIRONMENT_SET_SERIALIZATION_QUIRKS not implemented");
    return false;
}

bool Config::getAudioVideoEnable(int* enabled) {
    *enabled = true;
    return true;
}

bool Config::getFastForwarding(bool* is) {
    *is = false;
    return true;
}

bool Config::setCoreOptions(retro_core_option_definition const* options) {
    _logger->info("Setting core options");

    for (; options->key != nullptr; options++) {
        auto const found = _options.find(options->key);

        if (found == _options.end()) {
            _options.emplace(options->key, options->default_value);
        }

        _logger->info("    %s set to \"%s\"", options->key, _options[options->key].c_str());
    }

    return true;
}

bool Config::setCoreOptionsIntl(retro_core_options_intl const* intl) {
    _logger->warn("Using English for the core options");
    return setCoreOptions(intl->us);
}

bool Config::setCoreOptionsDisplay(retro_core_option_display const* display) {
    (void)display;
    _logger->warn("RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY not implemented");
    return false;
}

bool Config::initCoreDir(std::string const& corePath) {
    char path[PATH_MAX];

    if (realpath(corePath.c_str(), path) == NULL) {
        _logger->error("Error getting the absolute path to \"%s\": %s", corePath.c_str(), strerror(errno));
        destroy();
        return false;
    }

    char* const slash = strrchr(path, '/');
    char* const bslash = strrchr(path, '\\');

    if (slash != nullptr && bslash != nullptr) {
        (slash > bslash ? slash : bslash)[1] = 0;
    }
    else if (slash != nullptr) {
        slash[1] = 0;
    }
    else if (bslash != nullptr) {
        bslash[1] = 0;
    }
    else {
        _logger->error("Invalid core path \"%s\"", corePath.c_str());
        return false;
    }

    _coreDir = path;
    return true;
}

bool Config::initOptions(std::string const& settingsPath) {
    FILE* const file = fopen(settingsPath.c_str(), "r");

    if (file == nullptr) {
        _logger->error("Error opening \"%s\" for reading: %s", settingsPath.c_str(), strerror(errno));
        return false;
    }

    _logger->info("Reading settings from \"%s\"", settingsPath.c_str());

    char line[1024];

    while (fgets(line, sizeof(line), file) != nullptr) {
        line[sizeof(line) - 1] = 0;

        const char* keyBegin = line;

        while ((*keyBegin == ' ' || *keyBegin == '\t') && (*keyBegin != '\n' && *keyBegin != 0)) {
            keyBegin++;
        }

        if (*keyBegin == '\n' || *keyBegin == 0) {
            // Empty line.
            continue;
        }

        const char* keyEnd = keyBegin;

        while ((*keyEnd != ' ' && *keyEnd != '\t' && *keyEnd != '=') && (*keyEnd != '\n' && *keyEnd != 0)) {
            keyEnd++;
        }

        if (*keyEnd == '\n' || *keyEnd == 0) {
            _logger->error("Missing value for key \"%.*s\"", keyBegin, (int)(keyEnd - keyBegin));
            fclose(file);
            return false;
        }

        const std::string key(keyBegin, keyEnd - keyBegin);
        const char* valueBegin = keyEnd;

        while ((*valueBegin == ' ' || *valueBegin == '\t') && (*valueBegin != '=' && *valueBegin != '\n' && *valueBegin != 0)) {
            valueBegin++;
        }

        if (*valueBegin != '=') {
            _logger->error("Missing value for key \"%s\"", key.c_str());
            fclose(file);
            return false;
        }

        valueBegin++;

        while ((*valueBegin == ' ' || *valueBegin == '\t') && (*valueBegin != '"' && *valueBegin != '\n' && *valueBegin != 0)) {
            valueBegin++;
        }

        if (*valueBegin != '"') {
            _logger->error("Missing value for key \"%s\"", key.c_str());
            fclose(file);
            return false;
        }

        valueBegin++;
        const char* valueEnd = valueBegin;

        while (*valueEnd != '"' && *valueEnd != '\n' && *valueEnd != 0) {
            valueEnd++;
        }

        if (*valueEnd != '"') {
            _logger->error("Unterminated string for value of key \"%s\"", key.c_str());
            fclose(file);
            return false;
        }

        if (_options.find(key) != _options.end()) {
            _logger->warn("Duplicated key \"%s\"", key.c_str());
        }
        else {
            const std::string value(valueBegin, valueEnd - valueBegin);
            _logger->debug("Found key \"%s\" with value \"%s\"", key.c_str(), value.c_str());
            _options.emplace(key, value);
        }
    }

    if (ferror(file)) {
        _logger->error("Error reading from configuration file");
        fclose(file);
        return false;
    }

    fclose(file);
    return true;
}

void Config::reset() {
    _logger = nullptr;

    _systemDir.clear();
    _coreDir.clear();
    _assetsDir.clear();
    _savesDir.clear();
    _supportsNoGame = false;

    _options.clear();
    _optionsUpdated =false;
}
