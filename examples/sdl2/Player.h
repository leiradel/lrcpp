#pragma once

#include "Logger.h"
#include "Perf.h"
#include "Config.h"
#include "Audio.h"
#include "Video.h"
#include "Input.h"

#include <string>
#include <vector>

class Player {
public:
    bool init(std::vector<std::string> const& configPaths, char const* corePath, char const* contentPath, int verboseness);
    void destroy();

    void run();

protected:
    void const* readAll(char const* path, size_t* size);

    Logger _logger;
    Perf _perf;
    Config _config;
    Audio _audio;
    Video _video;
    Input _input;
};
