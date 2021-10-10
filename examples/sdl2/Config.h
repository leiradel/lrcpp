#pragma once

#include <lrcpp/Components.h>

#include <vector>
#include <unordered_map>
#include <string>

class Config : public lrcpp::Config {
public:
    Config();

    bool init(char const* configPath, char const* contentPath, char const* corePath, lrcpp::Logger* logger);
    void destroy();

    bool loadOptions(char const* configPath);
    bool getOption(char const* key, char const** value) const;

    // lrcpp::Config
    virtual bool setPerformanceLevel(unsigned level) override;
    virtual bool getSystemDirectory(char const** directory) override;
    virtual bool getVariable(retro_variable* variable) override;
    virtual bool getVariableUpdate(bool* const updated) override;
    virtual bool setSupportNoGame(bool const supports) override;
    virtual bool getLibretroPath(char const** path) override;
    virtual bool getCoreAssetsDirectory(char const** directory) override;
    virtual bool getSaveDirectory(char const** directory) override;
    virtual bool setProcAddressCallback(retro_get_proc_address_interface const* callback) override;
    virtual bool setSubsystemInfo(retro_subsystem_info const* info) override;
    virtual bool setMemoryMaps(retro_memory_map const* map) override;
    virtual bool getUsername(char const** username) override;
    virtual bool getLanguage(unsigned* language) override;
    virtual bool setSupportAchievements(bool supports) override;
    virtual bool setSerializationQuirks(uint64_t quirks) override;
    virtual bool getAudioVideoEnable(int* enabled) override;
    virtual bool getFastForwarding(bool* is) override;
    virtual bool setCoreOptions(retro_core_option_definition const* options) override;
    virtual bool setCoreOptionsIntl(retro_core_options_intl const* intl) override;
    virtual bool setCoreOptionsDisplay(retro_core_option_display const* display) override;

protected:
    bool getDirectory(char const* path, std::string* directory);
    void reset();

    lrcpp::Logger* _logger;

    std::string _contentDir;
    std::string _coreDir;
    bool _supportsNoGame;

    std::unordered_map<std::string, std::string> _options;
    bool _optionsUpdated;
};
