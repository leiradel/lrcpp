#include "Player.h"

#include <errno.h>
#include <string.h>
#include <sys/stat.h>

Player::Player() {
    _emuThread = nullptr;
}

bool Player::loadCore(char const *path)
{
    if (!_dynlib.load(path)) {
        _logger.error("Failed to load library: \n");
        return false;
    }

#define LOAD_CORE_FUNC(member_name, name) \
    _core.member_name = reinterpret_cast<decltype(_core.member_name)>(_dynlib.getSymbol("retro_" #name)); \
    if (!_core.member_name) { \
        _logger.error("Missing symbol: retro_" #name "\n"); \
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

bool Player::init(std::vector<std::string> const& configPaths) {

    if (!_logger.init()) {
        // Doesn't really happen
        return false;
    }

    _logger.setLevel(RETRO_LOG_WARN);

    if (!_config.init(configPaths, &_logger)) {
        _logger.error("Could not initialize the configuration component\n");
        _logger.destroy();
        return false;
    }

    if (_config.hasOption("sdl2lrcpp_log_path")) {
        char const* logPath = nullptr;
        _config.getOption("sdl2lrcpp_log_path", &logPath);
        _logger.setLogPath(logPath);
    }

    if (_config.hasOption("sdl2lrcpp_log_level")) {
        char const* level = nullptr;
        _config.getOption("sdl2lrcpp_log_level", &level);

        if (strcmp(level, "debug") == 0) {
            _logger.setLevel(RETRO_LOG_DEBUG);
        }
        else if (strcmp(level, "info") == 0) {
            _logger.setLevel(RETRO_LOG_INFO);
        }
        else if (strcmp(level, "warn") == 0) {
            _logger.setLevel(RETRO_LOG_WARN);
        }
        else if (strcmp(level, "error") == 0) {
            _logger.setLevel(RETRO_LOG_ERROR);
        }
        else {
            _logger.warn("Invalid sdl2lrcpp_log_level \"%s\", keeping warn\n", level);
        }
    }

    if (!_config.resolvePaths()) {
        _logger.error("Could not resolve the core and content directories\n");
        _config.destroy();
        _logger.destroy();
        return false;
    }

    char const* corePath = nullptr;
    _config.getOption("sdl2lrcpp_core_path", &corePath);

    char const* contentPath = nullptr;

    if (_config.hasOption("sdl2lrcpp_content_path")) {
        _config.getOption("sdl2lrcpp_content_path", &contentPath);
    }

    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
        _logger.error("SDL_InitSubSystem(SDL_INIT_EVENTS) failed: %s\n", SDL_GetError());
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!_perf.init()) {
        _logger.error("Could not initialize the perf component\n");
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!_audio.init(&_config, &_logger)) {
        _logger.error("Could not initialize the audio component\n");
        _perf.destroy();
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!_video.init(&_config, &_logger)) {
        _logger.error("Could not initialize the video component\n");
        _audio.destroy();
        _perf.destroy();
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    if (!_input.init(&_logger)) {
        _logger.error("Could not initialize the input component\n");
        _video.destroy();
        _audio.destroy();
        _perf.destroy();
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        _config.destroy();
        _logger.destroy();
        SDL_Quit();
        return false;
    }

    _vfs.init(&_logger);

    if (!_frontend.setLogger(&_logger) || !_frontend.setConfig(&_config) || !_frontend.setVideo(&_video)) {
        _logger.error("Could not set components in the frontend\n");

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

    if (!_frontend.setPerf(&_perf) || !_frontend.setAudio(&_audio) || !_frontend.setInput(&_input) ||
        !_frontend.setVirtualFileSystem(&_vfs)) {
        _logger.error("Could not set components in the frontend\n");
        goto error;
    }

    _logger.info("Loading core from \"%s\"\n", corePath);

    if (!loadCore(corePath)) {
        _logger.error("Failed to initialize core.\n");
        goto error;
    }

    if (!_frontend.setCore(&_core)) {
        _logger.error("Could not load the core from \"%s\"\n", corePath);
        _dynlib.unload();
        goto error;
    }

    retro_system_info sysinfo;

    if (!_frontend.getSystemInfo(&sysinfo)) {
        _logger.error("Could not get the system info from the core\n");
        _frontend.unset();
        _dynlib.unload();
        goto error;
    }

    _logger.info("System Info\n");
    _logger.info("    library_name     = \"%s\"\n", sysinfo.library_name);
    _logger.info("    library_version  = %s\n", sysinfo.library_version);
    _logger.info("    valid_extensions = \"%s\"\n", sysinfo.valid_extensions);
    _logger.info("    need_fullpath    = %s\n", sysinfo.need_fullpath ? "true" : "false");
    _logger.info("    block_extract    = %s\n", sysinfo.block_extract ? "true" : "false");

    bool ok = false;

    if (contentPath != nullptr) {
        _logger.info("Loading content from \"%s\"\n", contentPath);

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
    }
    else {
        _logger.info("No content path set, starting the core without content\n");
        ok = _frontend.loadGame();
    }

    if (!ok) {
        _logger.error("Could not load the content\n");
        _frontend.unset();
        _dynlib.unload();
        goto error;
    }

    if (_config.hasOption("sdl2lrcpp_state_path")) {
        char const* statePath = nullptr;
        _config.getOption("sdl2lrcpp_state_path", &statePath);

        _logger.info("Loading state from \"%s\"\n", statePath);

        size_t size = 0;
        void const* data = readAll(statePath, &size);

        if (data == nullptr) {
            _logger.error("Could not read state from \"%s\"\n", statePath);
        }
        else {
            if (!_frontend.unserialize(data, size)) {
                _logger.error("Could not load state from \"%s\"\n", statePath);
            }

            free(const_cast<void*>(data));
        }
    }

    return true;
}

void Player::destroy() {
    _frontend.unloadGame();
    _frontend.unset();

    _input.destroy();
    _vfs.destroy();
    _video.destroy();
    _audio.destroy();
    _config.destroy();
    _perf.destroy();
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    _logger.destroy();

    _dynlib.unload();

    SDL_Quit();
}

int Player::emulationThread(void* userdata) {
    Player* const self = static_cast<Player*>(userdata);

    while (self->_audio.waitToFill()) {
        self->_frontend.run();
    }

    return 0;
}

void Player::run() {
    _emuThread = SDL_CreateThread(emulationThread, "emulation", this);

    if (_emuThread == nullptr) {
        _logger.error("SDL_CreateThread() failed: %s\n", SDL_GetError());
        return;
    }

    bool done = false;

    do {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = true;
            }
            else {
                _input.process(&event);
            }
        }

        _video.present();
    }
    while (!done);

    _audio.signalStop();
    SDL_WaitThread(_emuThread, nullptr);
    _emuThread = nullptr;
}

void const* Player::readAll(char const* path, size_t* size) {
    struct stat statbuf;

    if (stat(path, &statbuf) != 0) {
        _logger.error("Error getting content info: %s\n", strerror(errno));
        return nullptr;
    }

    void* data = malloc(statbuf.st_size);

    if (data == nullptr) {
        _logger.error("Out of memory allocating %zu bytes\n", statbuf.st_size);
        return nullptr;
    }

    FILE* file = fopen(path, "rb");

    if (file == nullptr) {
        _logger.error("Error opening content: %s\n", strerror(errno));
        free(data);
        return nullptr;
    }

    size_t numread = fread(data, 1, statbuf.st_size, file);

    if (numread != (size_t)statbuf.st_size) {
        _logger.error("Error reading content: %s\n", strerror(errno));
        fclose(file);
        free(data);
        return nullptr;
    }

    fclose(file);

    _logger.debug("Loaded data from \"%s\", %zu bytes\n", path, numread);
    *size = numread;
    return data;
}
