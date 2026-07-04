#pragma once

#include <lrcpp/Components.h>

#include <vector>
#include <map>
#include <string>
#include <vector>

class Config : public lrcpp::Config {
public:
    Config();

    bool init(std::vector<std::string> const& configPaths, lrcpp::Logger* logger);
    void destroy();

    bool resolvePaths();

    bool hasOption(char const* key) const;
    bool getOption(char const* key, char const** value) const;
    bool getOption(char const* key, unsigned long* value) const;
    bool getOption(char const* key, bool* value) const;

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
    virtual bool setCoreOptionsV2Intl(retro_core_options_v2_intl const* intlv2) override;
    virtual bool setCoreOptionsDisplay(retro_core_option_display const* display) override;

protected:
    bool getDirectory(char const* path, std::string* directory);
    bool loadOptions(char const* configPath);
    void reset();

    lrcpp::Logger* _logger;

    std::string _contentDir;
    std::string _coreDir;
    bool _supportsNoGame;

    std::map<std::string, std::string> _options;
    bool _optionsUpdated;
};
