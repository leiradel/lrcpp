#ifndef COREFSM_H__
#define COREFSM_H__

/* Generated with FSM compiler: https://github.com/leiradel/luamods/ddlt */

#include <stdarg.h>


    #include <lrcpp/libretro.h>

    namespace lrcpp {
        struct Core;
        class Frontend;
    }

    typedef lrcpp::Core* CorePtr;
    typedef lrcpp::Frontend* FrontendPtr;
    typedef size_t* SizePtr;
    typedef char const* ConstCharPtr;
    typedef retro_game_info const* ConstRetroGameInfoPtr;
    typedef void* VoidPtr;
    typedef void const* ConstVoidPtr;
    typedef void** VoidPtrPtr;
    typedef unsigned* UnsignedPtr;
    typedef struct retro_system_info* RetroSystemInfoPtr;
    typedef struct retro_system_av_info* RetroSystemAvInfoPtr;


/* FSM states */
typedef enum {
    COREFSM_STATE_CORE_INITIALIZED,
    COREFSM_STATE_CORE_SET,
    COREFSM_STATE_ENVIRONMENT_SET,
    COREFSM_STATE_GAME_LOADED,
    COREFSM_STATE_GAME_RUNNING,
    COREFSM_STATE_START,
}
CoreFsm_State;

/* FSM transitions */
typedef enum {
    COREFSM_TRANSITION_API_VERSION,
    COREFSM_TRANSITION_CHEAT_RESET,
    COREFSM_TRANSITION_CHEAT_SET,
    COREFSM_TRANSITION_CORE_SET,
    COREFSM_TRANSITION_DEINIT,
    COREFSM_TRANSITION_GET_MEMORY_DATA,
    COREFSM_TRANSITION_GET_MEMORY_SIZE,
    COREFSM_TRANSITION_GET_REGION,
    COREFSM_TRANSITION_GET_SYSTEM_AV_INFO,
    COREFSM_TRANSITION_GET_SYSTEM_INFO,
    COREFSM_TRANSITION_GOTO_CORE_SET,
    COREFSM_TRANSITION_INIT,
    COREFSM_TRANSITION_LOAD_GAME,
    COREFSM_TRANSITION_LOAD_GAME_SPECIAL,
    COREFSM_TRANSITION_RESET,
    COREFSM_TRANSITION_RUN,
    COREFSM_TRANSITION_SERIALIZE,
    COREFSM_TRANSITION_SERIALIZE_SIZE,
    COREFSM_TRANSITION_SET_CALLBACKS,
    COREFSM_TRANSITION_SET_CONTROLLER_PORT_DEVICE,
    COREFSM_TRANSITION_SET_ENVIRONMENT,
    COREFSM_TRANSITION_UNLOAD_GAME,
    COREFSM_TRANSITION_UNSERIALIZE,
    COREFSM_TRANSITION_UNSET,
}
CoreFsm_Transition;

typedef void (*CoreFsm_Vprintf)(void* const ud, char const* const fmt, va_list args);

/* The FSM */
typedef struct {
    CoreFsm_State state;
    CorePtr core;
    FrontendPtr frontend;
    FrontendPtr previous;

#ifdef DEBUG_FSM
    /* Set those after calling CoreFsm_Init when DEBUG_FSM is defined */
    CoreFsm_Vprintf vprintf;
    void* vprintf_ud;
#endif
}
CoreFsm_Context;

/* Initialization */
void CoreFsm_Init(CoreFsm_Context* const self, CorePtr const core, FrontendPtr const frontend, FrontendPtr const previous);

/* Query */
CoreFsm_State CoreFsm_CurrentState(CoreFsm_Context const* const self);
int CoreFsm_CanTransitionTo(CoreFsm_Context const* const self, CoreFsm_State const next);
int CoreFsm_CanUseTransition(CoreFsm_Context const* const self, CoreFsm_Transition const transition);

/* Transitions */
int CoreFsm_Transition_apiVersion(CoreFsm_Context* const self, UnsignedPtr version);
int CoreFsm_Transition_cheatReset(CoreFsm_Context* const self);
int CoreFsm_Transition_cheatSet(CoreFsm_Context* const self, unsigned index, bool enabled, ConstCharPtr code);
int CoreFsm_Transition_coreSet(CoreFsm_Context* const self);
int CoreFsm_Transition_deinit(CoreFsm_Context* const self);
int CoreFsm_Transition_getMemoryData(CoreFsm_Context* const self, unsigned id, VoidPtrPtr data);
int CoreFsm_Transition_getMemorySize(CoreFsm_Context* const self, unsigned id, SizePtr size);
int CoreFsm_Transition_getRegion(CoreFsm_Context* const self, UnsignedPtr region);
int CoreFsm_Transition_getSystemAvInfo(CoreFsm_Context* const self, RetroSystemAvInfoPtr info);
int CoreFsm_Transition_getSystemInfo(CoreFsm_Context* const self, RetroSystemInfoPtr info);
int CoreFsm_Transition_gotoCoreSet(CoreFsm_Context* const self);
int CoreFsm_Transition_init(CoreFsm_Context* const self);
int CoreFsm_Transition_loadGame(CoreFsm_Context* const self, ConstRetroGameInfoPtr gameInfo);
int CoreFsm_Transition_loadGameSpecial(CoreFsm_Context* const self, unsigned gameType, ConstRetroGameInfoPtr info, size_t numInfo);
int CoreFsm_Transition_reset(CoreFsm_Context* const self);
int CoreFsm_Transition_run(CoreFsm_Context* const self);
int CoreFsm_Transition_serialize(CoreFsm_Context* const self, VoidPtr data, size_t size);
int CoreFsm_Transition_serializeSize(CoreFsm_Context* const self, SizePtr size);
int CoreFsm_Transition_setCallbacks(CoreFsm_Context* const self, retro_video_refresh_t videoRefresh, retro_audio_sample_t audioSample, retro_audio_sample_batch_t audioSampleBatch, retro_input_poll_t inputPoll, retro_input_state_t inputState);
int CoreFsm_Transition_setControllerPortDevice(CoreFsm_Context* const self, unsigned port, unsigned device);
int CoreFsm_Transition_setEnvironment(CoreFsm_Context* const self, retro_environment_t cb);
int CoreFsm_Transition_unloadGame(CoreFsm_Context* const self);
int CoreFsm_Transition_unserialize(CoreFsm_Context* const self, ConstVoidPtr data, size_t size);
int CoreFsm_Transition_unset(CoreFsm_Context* const self);

/* Debug */
#ifdef DEBUG_FSM
char const* CoreFsm_StateName(CoreFsm_State const state);
char const* CoreFsm_TransitionName(CoreFsm_Transition const transition);
#endif

#ifdef __cplusplus
class CoreFsm {
public:
    enum class State {
        CoreInitialized = COREFSM_STATE_CORE_INITIALIZED,
        CoreSet = COREFSM_STATE_CORE_SET,
        EnvironmentSet = COREFSM_STATE_ENVIRONMENT_SET,
        GameLoaded = COREFSM_STATE_GAME_LOADED,
        GameRunning = COREFSM_STATE_GAME_RUNNING,
        Start = COREFSM_STATE_START,
    };

    enum class Transition {
        apiVersion = COREFSM_TRANSITION_API_VERSION,
        cheatReset = COREFSM_TRANSITION_CHEAT_RESET,
        cheatSet = COREFSM_TRANSITION_CHEAT_SET,
        coreSet = COREFSM_TRANSITION_CORE_SET,
        deinit = COREFSM_TRANSITION_DEINIT,
        getMemoryData = COREFSM_TRANSITION_GET_MEMORY_DATA,
        getMemorySize = COREFSM_TRANSITION_GET_MEMORY_SIZE,
        getRegion = COREFSM_TRANSITION_GET_REGION,
        getSystemAvInfo = COREFSM_TRANSITION_GET_SYSTEM_AV_INFO,
        getSystemInfo = COREFSM_TRANSITION_GET_SYSTEM_INFO,
        gotoCoreSet = COREFSM_TRANSITION_GOTO_CORE_SET,
        init = COREFSM_TRANSITION_INIT,
        loadGame = COREFSM_TRANSITION_LOAD_GAME,
        loadGameSpecial = COREFSM_TRANSITION_LOAD_GAME_SPECIAL,
        reset = COREFSM_TRANSITION_RESET,
        run = COREFSM_TRANSITION_RUN,
        serialize = COREFSM_TRANSITION_SERIALIZE,
        serializeSize = COREFSM_TRANSITION_SERIALIZE_SIZE,
        setCallbacks = COREFSM_TRANSITION_SET_CALLBACKS,
        setControllerPortDevice = COREFSM_TRANSITION_SET_CONTROLLER_PORT_DEVICE,
        setEnvironment = COREFSM_TRANSITION_SET_ENVIRONMENT,
        unloadGame = COREFSM_TRANSITION_UNLOAD_GAME,
        unserialize = COREFSM_TRANSITION_UNSERIALIZE,
        unset = COREFSM_TRANSITION_UNSET,
    };

    CoreFsm(CorePtr const core, FrontendPtr const frontend, FrontendPtr const previous) { CoreFsm_Init(&_fsm, core, frontend, previous); }

#ifdef DEBUG_FSM
    CoreFsm(CorePtr const core, FrontendPtr const frontend, FrontendPtr const previous, CoreFsm_Vprintf vprintf, void* vprintf_ud) : CoreFsm(core, frontend, previous) {
        _fsm.vprintf = vprintf;
        _fsm.vprintf_ud = vprintf_ud;
    }
#endif

    State currentState() const { return (State)CoreFsm_CurrentState(&_fsm); }
    bool canTransitionTo(State const next) const { return CoreFsm_CanTransitionTo(&_fsm, (CoreFsm_State)next) != 0; }
    bool canUseTransition(Transition const transition) const { return CoreFsm_CanUseTransition(&_fsm, (CoreFsm_Transition)transition) != 0; }

    bool apiVersion(UnsignedPtr version) { return CoreFsm_Transition_apiVersion(&_fsm, version) != 0; }
    bool cheatReset() { return CoreFsm_Transition_cheatReset(&_fsm) != 0; }
    bool cheatSet(unsigned index, bool enabled, ConstCharPtr code) { return CoreFsm_Transition_cheatSet(&_fsm, index, enabled, code) != 0; }
    bool coreSet() { return CoreFsm_Transition_coreSet(&_fsm) != 0; }
    bool deinit() { return CoreFsm_Transition_deinit(&_fsm) != 0; }
    bool getMemoryData(unsigned id, VoidPtrPtr data) { return CoreFsm_Transition_getMemoryData(&_fsm, id, data) != 0; }
    bool getMemorySize(unsigned id, SizePtr size) { return CoreFsm_Transition_getMemorySize(&_fsm, id, size) != 0; }
    bool getRegion(UnsignedPtr region) { return CoreFsm_Transition_getRegion(&_fsm, region) != 0; }
    bool getSystemAvInfo(RetroSystemAvInfoPtr info) { return CoreFsm_Transition_getSystemAvInfo(&_fsm, info) != 0; }
    bool getSystemInfo(RetroSystemInfoPtr info) { return CoreFsm_Transition_getSystemInfo(&_fsm, info) != 0; }
    bool gotoCoreSet() { return CoreFsm_Transition_gotoCoreSet(&_fsm) != 0; }
    bool init() { return CoreFsm_Transition_init(&_fsm) != 0; }
    bool loadGame(ConstRetroGameInfoPtr gameInfo) { return CoreFsm_Transition_loadGame(&_fsm, gameInfo) != 0; }
    bool loadGameSpecial(unsigned gameType, ConstRetroGameInfoPtr info, size_t numInfo) { return CoreFsm_Transition_loadGameSpecial(&_fsm, gameType, info, numInfo) != 0; }
    bool reset() { return CoreFsm_Transition_reset(&_fsm) != 0; }
    bool run() { return CoreFsm_Transition_run(&_fsm) != 0; }
    bool serialize(VoidPtr data, size_t size) { return CoreFsm_Transition_serialize(&_fsm, data, size) != 0; }
    bool serializeSize(SizePtr size) { return CoreFsm_Transition_serializeSize(&_fsm, size) != 0; }
    bool setCallbacks(retro_video_refresh_t videoRefresh, retro_audio_sample_t audioSample, retro_audio_sample_batch_t audioSampleBatch, retro_input_poll_t inputPoll, retro_input_state_t inputState) { return CoreFsm_Transition_setCallbacks(&_fsm, videoRefresh, audioSample, audioSampleBatch, inputPoll, inputState) != 0; }
    bool setControllerPortDevice(unsigned port, unsigned device) { return CoreFsm_Transition_setControllerPortDevice(&_fsm, port, device) != 0; }
    bool setEnvironment(retro_environment_t cb) { return CoreFsm_Transition_setEnvironment(&_fsm, cb) != 0; }
    bool unloadGame() { return CoreFsm_Transition_unloadGame(&_fsm) != 0; }
    bool unserialize(ConstVoidPtr data, size_t size) { return CoreFsm_Transition_unserialize(&_fsm, data, size) != 0; }
    bool unset() { return CoreFsm_Transition_unset(&_fsm) != 0; }

#ifdef DEBUG_FSM
    static char const* stateName(State const state) { return CoreFsm_StateName((CoreFsm_State)state); }
    static char const* transitionName(Transition const transition) { return CoreFsm_TransitionName((CoreFsm_Transition)transition); }
#endif

protected:
    CoreFsm_Context _fsm;
};
#endif

#endif /* COREFSM_H__ */
