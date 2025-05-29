#include "Player.h"

#include <errno.h>
#include <sys/stat.h>

bool Player::loadCore(char const *path)
{
    if (!_dynlib.load(path)) {
        _logger.error("Failed to load library: ");
        return false;
    }

#define LOAD_CORE_FUNC(member_name, name) \
    _core.member_name = reinterpret_cast<decltype(_core.member_name)>(_dynlib.getSymbol("retro_" #name)); \
    if (!_core.member_name) { \
        _logger.error("Missing symbol: retro_" #name); \
        return false; \
    }

    LOAD_CORE_FUNC(init, init)
    LOAD_CORE_FUNC(deinit, deinit)
    LOAD_CORE_FUNC(apiVersion, api_version)
    LOAD_CORE_FUNC(getSystemInfo, get_system_info)
    LOAD_CORE_FUNC(getSystemAvInfo, get_system_av_info)
    LOAD_CORE_FUNC(setEnvironment, set_environment)
    LOAD_CORE_FUNC(setVideoRefresh, set_video_refresh)
    LOAD_CORE_FUNC(setAudioSample, set_audio_sample)
    LOAD_CORE_FUNC(setAudioSampleBatch, set_audio_sample_batch)
    LOAD_CORE_FUNC(setInputPoll, set_input_poll)
    LOAD_CORE_FUNC(setInputState, set_input_state)
    LOAD_CORE_FUNC(setControllerPortDevice, set_controller_port_device)
    LOAD_CORE_FUNC(reset, reset)
    LOAD_CORE_FUNC(run, run)
    LOAD_CORE_FUNC(serializeSize, serialize_size)
    LOAD_CORE_FUNC(serialize, serialize)
    LOAD_CORE_FUNC(unserialize, unserialize)
    LOAD_CORE_FUNC(cheatReset, cheat_reset)
    LOAD_CORE_FUNC(cheatSet, cheat_set)
    LOAD_CORE_FUNC(loadGame, load_game)
    LOAD_CORE_FUNC(loadGameSpecial, load_game_special)
    LOAD_CORE_FUNC(unloadGame, unload_game)
    LOAD_CORE_FUNC(getRegion, get_region)
    LOAD_CORE_FUNC(getMemoryData, get_memory_data)
    LOAD_CORE_FUNC(getMemorySize, get_memory_size)

#undef LOAD_CORE_FUNC

    // If needed: keep lib alive by returning it or capturing in a smart pointer
    return true;
}

bool Player::init(std::vector<std::string> const& configPaths, char const* corePath, char const* contentPath, int verboseness) {

    if (!_logger.init()) {
        // Doesn't really happen
        return false;
    }

    _logger.setLevel(RETRO_LOG_WARN);

    if (!_config.init(configPaths, contentPath, corePath, &_logger)) {
        _logger.error("Could not initialize the configuration component");
        _logger.destroy();
        return false;
    }

    {
        unsigned long level = 2;
        _config.getOption("libretro_log_level", &level);
        long realLevel = (long)level - verboseness;

        if (realLevel <= 0) {
            _logger.setLevel(RETRO_LOG_DEBUG);
        }
        else if (realLevel == 1) {
            _logger.setLevel(RETRO_LOG_INFO);
        }
        else if (realLevel == 2) {
            _logger.setLevel(RETRO_LOG_WARN);
        }
        else if (realLevel >= 3) {
            _logger.setLevel(RETRO_LOG_ERROR);
        }
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

    if (!_audio.init(&_config, &_logger)) {
        _logger.error("Could not initialize the audio component");
        _perf.destroy();
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!_video.init(&_config, &_logger)) {
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

    if (!_frontend.setLogger(&_logger) || !_frontend.setConfig(&_config) || !_frontend.setVideo(&_video)) {
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

    if (!_frontend.setPerf(&_perf) || !_frontend.setAudio(&_audio) || !_frontend.setInput(&_input)) {
        _logger.error("Could not set components in the frontend");
        goto error;
    }

    _logger.info("Loading core from \"%s\"", corePath);

    if (!loadCore(corePath)) {
        _logger.error("Failed to initialize core.");
        goto error;
    }

    if (!_frontend.setCore(&_core)) {
        _logger.error("Could not load the core from \"%s\"", corePath);
        goto error;
    }

    retro_system_info sysinfo;

    if (!_frontend.getSystemInfo(&sysinfo)) {
        _logger.error("Could not get the system info from the core");
        _frontend.unset();
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
        ok = _frontend.loadGame(contentPath);
    }
    else {
        size_t size = 0;
        void const* data = readAll(contentPath, &size);

        if (data != nullptr) {
            ok = _frontend.loadGame(contentPath, data, size);
            free(const_cast<void*>(data));
        }
    }

    if (!ok) {
        _logger.error("Could not load content from \"%s\"", contentPath);
        _frontend.unset();
        goto error;
    }

    return true;
}

void Player::destroy() {
    _frontend.unloadGame();
    _frontend.unset();

    _input.destroy();
    _video.destroy();
    _audio.destroy();
    _config.destroy();
    _perf.destroy();
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    _logger.destroy();

    _dynlib.unload();

    SDL_Quit();
}

void Player::run() {
    uint64_t const coreUsPerFrame = 1000000 / _video.getCoreFps();
    uint64_t nextFrameTime = Perf::getTimeUs() + coreUsPerFrame;
    bool done = false;

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
            _frontend.run();
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
