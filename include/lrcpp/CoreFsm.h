#ifndef COREFSM_H__
#define COREFSM_H__

/* Generated with FSM compiler: https://github.com/leiradel/luamods/ddlt */

#include <stdarg.h>

/*#line 25 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

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
    CoreFsm_State_CoreInitialized,
    CoreFsm_State_CoreSet,
    CoreFsm_State_EnvironmentSet,
    CoreFsm_State_GameLoaded,
    CoreFsm_State_GameRunning,
    CoreFsm_State_Start,
}
CoreFsm_State;

/* The FSM */
typedef struct {
    CoreFsm_State state;
    CorePtr core;
    FrontendPtr frontend;
    FrontendPtr previous;

#ifdef DEBUG_FSM
    /* Set those after calling CoreFsm_Init when DEBUG_FSM is defined */
    void (*vprintf)(void* const ud, char const* const fmt, va_list args);
    void* vprintf_ud;
#endif
}
CoreFsm_Context;

/* Initialization */
void CoreFsm_Init(CoreFsm_Context* const self, CorePtr const core, FrontendPtr const frontend, FrontendPtr const previous);

/* Query */
CoreFsm_State CoreFsm_CurrentState(CoreFsm_Context const* const self);
int CoreFsm_CanTransitionTo(CoreFsm_Context const* const self, CoreFsm_State const next);

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
#endif

#endif /* COREFSM_H__ */
