#include "Config.h"

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifdef _WIN32
#define SEPARATOR '\\'
#define SEPARATOR2 '/'
#else
#define SEPARATOR '/'
#endif

Config::Config() {
    reset();
}

bool Config::init(std::vector<std::string> const& configPaths,
                  char const* contentPath,
                  char const* corePath,
                  lrcpp::Logger* logger) {

    reset();

    _logger = logger;

    for (auto const& path : configPaths) {
        _logger->info("Loading configuration from \"%s\"", path.c_str());

        if (!loadOptions(path.c_str())) {
            return false;
        }
    }

    if (!getDirectory(contentPath, &_contentDir)) {
        return false;
    }

    _logger->info("Content directory is \"%s\"", _contentDir.c_str());

    if (!getDirectory(corePath, &_coreDir)) {
        return false;
    }

    _logger->info("Core directory is \"%s\"", _coreDir.c_str());
    return true;
}

void Config::destroy() {
    reset();
}

bool Config::getOption(char const* key, char const** value) const {
    auto const it = _options.find(key);

    if (it == _options.end()) {
        _logger->error("Could't find \"%s\" in the configuration file", key);
        return false;
    }

    *value = it->second.c_str();

    _logger->debug("Found value \"%s\" for key \"%s\"", *value, key);
    return true;
}

bool Config::getOption(char const* key, unsigned long* value) const {
    char const* valueStr = nullptr;

    if (!getOption(key, &valueStr)) {
        return false;
    }

    char* endptr = nullptr;
    errno = 0;
    *value = strtoul(valueStr, &endptr, 0);
    return *valueStr != 0 && *endptr == 0 && errno == 0;
}

bool Config::setPerformanceLevel(unsigned level) {
    (void)level;
    _logger->warn("RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL not implemented");
    return false;
}

bool Config::getSystemDirectory(char const** directory) {
    *directory = _coreDir.c_str();
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
    *directory = _coreDir.c_str();
    return true;
}

bool Config::getSaveDirectory(char const** directory) {
    *directory = _contentDir.c_str();
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

bool Config::getDirectory(char const* path, std::string* directory) {
    size_t const length = strlen(path);

    if (length == 0) {
        _logger->error("Cannot get directory from an empty path");
        return false;
    }

    struct stat statbuf;

    if (stat(path, &statbuf) != 0) {
        _logger->error("Error info for path \"%s\": %s", path, strerror(errno));
        return false;
    }

    if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
        // It's a directory.
        *directory = path;

#ifdef SEPARATOR2
        if (path[length - 1] != SEPARATOR && path[length - 1] != SEPARATOR2) {
#else
        if (path[length - 1] != SEPARATOR) {
#endif
            directory->append(1, SEPARATOR);
        }
    }
    else {
        bool separator = false;

        // Assume it's a file.
        for (size_t i = length - 1; i > 0; i--) {
#ifdef SEPARATOR2
            if (path[i] == SEPARATOR || path[i] == SEPARATOR2) {
#else
            if (path[i] == SEPARATOR) {
#endif
                *directory = std::string(path, i + 1);
                separator = true;
                break;
            }
        }

        if (!separator) {
            *directory = ".";
        }
    }

    _logger->debug("Directory for \"%s\" is \"%s\"", path, directory->c_str());
    return true;
}

bool Config::loadOptions(char const* configPath) {
    _logger->info("Reading settings from \"%s\"", configPath);

    FILE* const file = fopen(configPath, "r");

    if (file == nullptr) {
        _logger->error("Error opening \"%s\" for reading: %s", configPath, strerror(errno));
        return false;
    }

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

    _contentDir.clear();
    _coreDir.clear();
    _supportsNoGame = false;

    _options.clear();
    _optionsUpdated =false;
}
