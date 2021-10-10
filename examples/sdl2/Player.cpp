#include "Player.h"

#include <lrcpp/Frontend.h>

#include <sys/stat.h>

bool Player::init(std::vector<std::string> const& configPaths,
                  char const* corePath,
                  char const* contentPath,
                  retro_log_level level) {

    if (!_logger.init()) {
        // Doesn't really happen
        return false;
    }

    _logger.setLevel(level);

    if (!_config.init(configPaths, contentPath, corePath, &_logger)) {
        _logger.error("Could not initialize the configuration component");
        _logger.destroy();
        return false;
    }

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
        _logger.error("SDL_InitSubSystem(SDL_INIT_EVENTS) failed: %s", SDL_GetError());
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!_perf.init()) {
        _logger.error("Could not initialize the perf component");
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!_audio.init(&_logger)) {
        _logger.error("Could not initialize the audio component");
        _perf.destroy();
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!_video.init(&_logger)) {
        _logger.error("Could not initialize the video component");
        _audio.destroy();
        _perf.destroy();
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!_input.init(&_logger)) {
        _logger.error("Could not initialize the input component");
        _video.destroy();
        _audio.destroy();
        _perf.destroy();
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    auto& frontend = lrcpp::Frontend::getInstance();

    if (!frontend.setLogger(&_logger) || !frontend.setConfig(&_config) || !frontend.setVideo(&_video)) {
        _logger.error("Could not set components in the frontend");

error:
        _input.destroy();
        _video.destroy();
        _audio.destroy();
        _perf.destroy();
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!frontend.setPerf(&_perf) || !frontend.setAudio(&_audio) || !frontend.setInput(&_input)) {
        _logger.error("Could not set components in the frontend");
        goto error;
    }

    _logger.info("Loading core from \"%s\"", corePath);

    if (!frontend.load(corePath)) {
        _logger.error("Could not load the core from \"%s\"", corePath);
        goto error;
    }

    retro_system_info sysinfo;

    if (!frontend.getSystemInfo(&sysinfo)) {
        _logger.error("Could not get the system info from the core");
        frontend.unload();
        goto error;
    }

    _logger.info("System Info");
    _logger.info("    library_name     = %s", sysinfo.library_name);
    _logger.info("    library_version  = %s", sysinfo.library_version);
    _logger.info("    valid_extensions = %s", sysinfo.valid_extensions);
    _logger.info("    need_fullpath    = %s", sysinfo.need_fullpath ? "true" : "false");
    _logger.info("    block_extract    = %s", sysinfo.block_extract ? "true" : "false");

    _logger.info("Loading content from \"%s\"", contentPath);

    bool ok = false;

    if (sysinfo.need_fullpath) {
        ok = frontend.loadGame(contentPath);
    }
    else {
        size_t size = 0;
        void const* data = readAll(contentPath, &size);

        if (data != nullptr) {
            ok = frontend.loadGame(contentPath, data, size);
            free(const_cast<void*>(data));
        }
    }

    if (!ok) {
        _logger.error("Could not load content from \"%s\"", contentPath);
        frontend.unload();
        goto error;
    }

    return true;
}

void Player::destroy() {
    auto& frontend = lrcpp::Frontend::getInstance();

    frontend.unloadGame();
    frontend.unload();

    _input.destroy();
    _video.destroy();
    _audio.destroy();
    _config.destroy();
    _perf.destroy();
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    _logger.destroy();

    SDL_Quit();
}

void Player::run() {
    uint64_t const coreUsPerFrame = 1000000 / _video.getCoreFps();
    uint64_t nextFrameTime = Perf::getTimeUs() + coreUsPerFrame;
    bool done = false;

    auto& frontend = lrcpp::Frontend::getInstance();

    do {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            // TODO process SDL events in the Input class

            if (event.type == SDL_QUIT) {
                done = true;
            }
            else {
                _input.process(&event);
            }
        }

        if (Perf::getTimeUs() >= nextFrameTime) {
            nextFrameTime += coreUsPerFrame;

            _audio.clear();
            _video.clear();
            frontend.run();
            _audio.present();
            _video.present();

            _logger.debug("Ran one frame");
        }

        // TODO pause for less time for greater granularity
        SDL_Delay(1);
    }
    while (!done);
}

void const* Player::readAll(char const* path, size_t* size) {
    struct stat statbuf;

    if (stat(path, &statbuf) != 0) {
        _logger.error("Error getting content info: %s", strerror(errno));
        return nullptr;
    }

    void* data = malloc(statbuf.st_size);

    if (data == nullptr) {
        _logger.error("Out of memory allocating %zu bytes", statbuf.st_size);
        return nullptr;
    }

    FILE* file = fopen(path, "rb");

    if (file == nullptr) {
        _logger.error("Error opening content: %s", strerror(errno));
        free(data);
        return nullptr;
    }

    size_t numread = fread(data, 1, statbuf.st_size, file);

    if (numread != (size_t)statbuf.st_size) {
        _logger.error("Error reading content: %s", strerror(errno));
        fclose(file);
        free(data);
        return nullptr;
    }

    fclose(file);

    _logger.debug("Loaded data from \"%s\", %zu bytes", path, numread);
    *size = numread;
    return data;
}
