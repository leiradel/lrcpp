#pragma once

#include "Logger.h"
#include "Perf.h"
#include "Config.h"
#include "Audio.h"
#include "Video.h"

class Player {
public:
    bool init(char const* configPath, char const* corePath, char const* contentPath, retro_log_level level);
    void destroy();

    void run();

protected:
    void const* readAll(char const* path, size_t* size);

    Logger _logger;
    Perf _perf;
    Config _config;
    Audio _audio;
    Video _video;
};
