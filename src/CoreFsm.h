#pragma once

// Generated with FSM compiler, https://github.com/leiradel/luamods/ddlt

//#line 25 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

    #include <lrcpp/libretro.h>

    namespace lrcpp {
        class Core;
    }

    typedef lrcpp::Core Core;
    typedef size_t* size_ptr_t;
    typedef char const* const_char_ptr_t;
    typedef retro_game_info const* const_retro_game_info_ptr_t;
    typedef void* void_ptr_t;
    typedef void const* const_void_ptr_t;
    typedef void** void_ptr_ptr_t;
    typedef unsigned* unsigned_ptr_t;
    typedef struct retro_system_info* retro_system_info_ptr_t;
    typedef struct retro_system_av_info* retro_system_av_info_ptr_t;


#include <stdarg.h>

class CoreFsm {
public:
    enum class State {
        CoreInitialized,
        CoreLoaded,
        EnvironmentSet,
        GameLoaded,
        GameRunning,
        Start,
    };

    typedef void (*VPrintf)(void* ud, const char* fmt, va_list args);

    CoreFsm(Core& ctx) : _ctx(ctx), __state(State::Start), __vprintf(nullptr), __vprintfud(nullptr) {}
    CoreFsm(Core& ctx, VPrintf printer, void* printerud) : _ctx(ctx), __state(State::Start), __vprintf(printer), __vprintfud(printerud) {}

    State currentState() const { return __state; }

#ifdef DEBUG_FSM
    const char* stateName(State state) const;
    void printf(const char* fmt, ...);
#endif

    bool apiVersion(unsigned_ptr_t version);
    bool cheatReset();
    bool cheatSet(unsigned index, bool enabled, const_char_ptr_t code);
    bool deinit();
    bool getMemoryData(unsigned id, void_ptr_ptr_t data);
    bool getMemorySize(unsigned id, size_ptr_t size);
    bool getRegion(unsigned_ptr_t region);
    bool getSystemAvInfo(retro_system_av_info_ptr_t info);
    bool getSystemInfo(retro_system_info_ptr_t info);
    bool gotoCoreLoaded();
    bool init();
    bool load(const_char_ptr_t path);
    bool loadGame(const_retro_game_info_ptr_t gameInfo);
    bool loadGameSpecial(unsigned gameType, const_retro_game_info_ptr_t info, size_t numInfo);
    bool reset();
    bool run();
    bool serialize(void_ptr_t data, size_t size);
    bool serializeSize(size_ptr_t size);
    bool setCallbacks(retro_video_refresh_t videoRefresh, retro_audio_sample_t audioSample, retro_audio_sample_batch_t audioSampleBatch, retro_input_poll_t inputPoll, retro_input_state_t inputState);
    bool setControllerPortDevice(unsigned port, unsigned device);
    bool setEnvironment(retro_environment_t cb);
    bool unload();
    bool unloadGame();
    bool unserialize(const_void_ptr_t data, size_t size);

protected:
    bool before() const;
    bool before(State state) const;
    void after() const;
    void after(State state) const;

    Core& _ctx;
    State __state;
    VPrintf __vprintf;
    void* __vprintfud;
};
