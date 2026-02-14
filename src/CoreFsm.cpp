/* Generated with FSM compiler: https://github.com/leiradel/luamods/ddlt */

#include <stddef.h>


    #include <lrcpp/CoreFsm.h>
    #include <lrcpp/Frontend.h>


#ifdef DEBUG_FSM
static void fsmprintf(CoreFsm_Context* const self, const char* fmt, ...) {
    if (self->vprintf != NULL) {
        va_list args;
        va_start(args, fmt);
        self->vprintf(self->vprintf_ud, fmt, args);
        va_end(args);
    }
}

#define PRINTF(...) do { fsmprintf(__VA_ARGS__); } while (0)
#else
#define PRINTF(...) do {} while (0)
#endif

/* Initialization */
void CoreFsm_Init(CoreFsm_Context* const self, CorePtr const core, FrontendPtr const frontend, FrontendPtr const previous) {
    self->state = COREFSM_STATE_START;

    self->core = core;
    self->frontend = frontend;
    self->previous = previous;
}

CoreFsm_State CoreFsm_CurrentState(CoreFsm_Context const* const self) {
    return self->state;
}

int CoreFsm_CanTransitionTo(CoreFsm_Context const* const self, CoreFsm_State const next) {
    switch (self->state) {
        case COREFSM_STATE_CORE_INITIALIZED:
            switch (next) {
                case COREFSM_STATE_CORE_INITIALIZED:
                case COREFSM_STATE_ENVIRONMENT_SET:
                case COREFSM_STATE_GAME_LOADED:
                case COREFSM_STATE_START:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_CORE_SET:
            switch (next) {
                case COREFSM_STATE_CORE_SET:
                case COREFSM_STATE_ENVIRONMENT_SET:
                case COREFSM_STATE_START:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_ENVIRONMENT_SET:
            switch (next) {
                case COREFSM_STATE_CORE_INITIALIZED:
                case COREFSM_STATE_CORE_SET:
                case COREFSM_STATE_ENVIRONMENT_SET:
                case COREFSM_STATE_START:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_GAME_LOADED:
            switch (next) {
                case COREFSM_STATE_CORE_INITIALIZED:
                case COREFSM_STATE_ENVIRONMENT_SET:
                case COREFSM_STATE_GAME_LOADED:
                case COREFSM_STATE_GAME_RUNNING:
                case COREFSM_STATE_START:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_GAME_RUNNING:
            switch (next) {
                case COREFSM_STATE_CORE_INITIALIZED:
                case COREFSM_STATE_ENVIRONMENT_SET:
                case COREFSM_STATE_GAME_RUNNING:
                case COREFSM_STATE_START:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_START:
            switch (next) {
                case COREFSM_STATE_CORE_SET:
                    return 1;
                default: break;
            }
            break;

        default: break;
    }

    return 0;
}

int CoreFsm_CanUseTransition(CoreFsm_Context const* const self, CoreFsm_Transition const transition) {
    switch (self->state) {
        case COREFSM_STATE_CORE_INITIALIZED:
            switch (transition) {
                case COREFSM_TRANSITION_API_VERSION:
                case COREFSM_TRANSITION_DEINIT:
                case COREFSM_TRANSITION_GET_SYSTEM_INFO:
                case COREFSM_TRANSITION_LOAD_GAME:
                case COREFSM_TRANSITION_LOAD_GAME_SPECIAL:
                case COREFSM_TRANSITION_SET_CONTROLLER_PORT_DEVICE:
                case COREFSM_TRANSITION_UNSET:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_CORE_SET:
            switch (transition) {
                case COREFSM_TRANSITION_API_VERSION:
                case COREFSM_TRANSITION_GET_SYSTEM_INFO:
                case COREFSM_TRANSITION_SET_ENVIRONMENT:
                case COREFSM_TRANSITION_UNSET:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_ENVIRONMENT_SET:
            switch (transition) {
                case COREFSM_TRANSITION_API_VERSION:
                case COREFSM_TRANSITION_GET_SYSTEM_INFO:
                case COREFSM_TRANSITION_GOTO_CORE_SET:
                case COREFSM_TRANSITION_INIT:
                case COREFSM_TRANSITION_SET_CONTROLLER_PORT_DEVICE:
                case COREFSM_TRANSITION_UNSET:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_GAME_LOADED:
            switch (transition) {
                case COREFSM_TRANSITION_API_VERSION:
                case COREFSM_TRANSITION_DEINIT:
                case COREFSM_TRANSITION_GET_MEMORY_DATA:
                case COREFSM_TRANSITION_GET_MEMORY_SIZE:
                case COREFSM_TRANSITION_GET_REGION:
                case COREFSM_TRANSITION_GET_SYSTEM_AV_INFO:
                case COREFSM_TRANSITION_GET_SYSTEM_INFO:
                case COREFSM_TRANSITION_SET_CALLBACKS:
                case COREFSM_TRANSITION_SET_CONTROLLER_PORT_DEVICE:
                case COREFSM_TRANSITION_UNLOAD_GAME:
                case COREFSM_TRANSITION_UNSET:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_GAME_RUNNING:
            switch (transition) {
                case COREFSM_TRANSITION_API_VERSION:
                case COREFSM_TRANSITION_CHEAT_RESET:
                case COREFSM_TRANSITION_CHEAT_SET:
                case COREFSM_TRANSITION_DEINIT:
                case COREFSM_TRANSITION_GET_MEMORY_DATA:
                case COREFSM_TRANSITION_GET_MEMORY_SIZE:
                case COREFSM_TRANSITION_GET_REGION:
                case COREFSM_TRANSITION_GET_SYSTEM_AV_INFO:
                case COREFSM_TRANSITION_GET_SYSTEM_INFO:
                case COREFSM_TRANSITION_RESET:
                case COREFSM_TRANSITION_RUN:
                case COREFSM_TRANSITION_SERIALIZE:
                case COREFSM_TRANSITION_SERIALIZE_SIZE:
                case COREFSM_TRANSITION_SET_CONTROLLER_PORT_DEVICE:
                case COREFSM_TRANSITION_UNLOAD_GAME:
                case COREFSM_TRANSITION_UNSERIALIZE:
                case COREFSM_TRANSITION_UNSET:
                    return 1;
                default: break;
            }
            break;

        case COREFSM_STATE_START:
            switch (transition) {
                case COREFSM_TRANSITION_CORE_SET:
                    return 1;
                default: break;
            }
            break;

        default: break;
    }

    return 0;
}

static int global_before(CoreFsm_Context* const self) {
    (void)self;

        self->previous = lrcpp::Frontend::getCurrent();
        lrcpp::Frontend::setCurrent(self->frontend);
    
    return 1;
}

static int local_before(CoreFsm_Context* const self) {
    (void)self;
    return 1;
}

static void global_after(CoreFsm_Context* const self) {
    (void)self;

        lrcpp::Frontend::setCurrent(self->previous);
        self->previous = nullptr;
    
}

static void local_after(CoreFsm_Context* const self) {
    (void)self;
}

int CoreFsm_Transition_apiVersion(CoreFsm_Context* const self, UnsignedPtr version) {
    switch (self->state) {
        case COREFSM_STATE_CORE_INITIALIZED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreInitialized", "apiVersion"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }


            *version = self->core->apiVersion();
        
            self->state = COREFSM_STATE_CORE_INITIALIZED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_CORE_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreSet", "apiVersion"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }


            *version = self->core->apiVersion();
        
            self->state = COREFSM_STATE_CORE_SET;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreSet"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_ENVIRONMENT_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "EnvironmentSet", "apiVersion"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }


            *version = self->core->apiVersion();
        
            self->state = COREFSM_STATE_ENVIRONMENT_SET;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameLoaded", "apiVersion"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }


            *version = self->core->apiVersion();
        
            self->state = COREFSM_STATE_GAME_LOADED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "apiVersion"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            *version = self->core->apiVersion();
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "apiVersion", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_cheatReset(CoreFsm_Context* const self) {
    switch (self->state) {
        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "cheatReset"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            self->core->cheatReset();
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "cheatReset", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_cheatSet(CoreFsm_Context* const self, unsigned index, bool enabled, ConstCharPtr code) {
    switch (self->state) {
        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "cheatSet"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            self->core->cheatSet(index, enabled, code);
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "cheatSet", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_coreSet(CoreFsm_Context* const self) {
    switch (self->state) {
        case COREFSM_STATE_START: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreSet", "coreSet"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            self->state = COREFSM_STATE_CORE_SET;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreSet"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "coreSet", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_deinit(CoreFsm_Context* const self) {
    switch (self->state) {
        case COREFSM_STATE_CORE_INITIALIZED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "EnvironmentSet", "deinit"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }


            self->core->deinit();
        
            self->state = COREFSM_STATE_ENVIRONMENT_SET;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "EnvironmentSet", "deinit"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            const int ok__ = CoreFsm_Transition_unloadGame(self) &&
                             CoreFsm_Transition_deinit(self);

            if (ok__) {
                local_after(self);
                global_after(self);

            }
            else {
                PRINTF(
                    self,
                    "FSM %s:%u Failed to transition to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );
            }

            return ok__;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "EnvironmentSet", "deinit"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            const int ok__ = CoreFsm_Transition_unloadGame(self) &&
                             CoreFsm_Transition_deinit(self);

            if (ok__) {
                local_after(self);
                global_after(self);

            }
            else {
                PRINTF(
                    self,
                    "FSM %s:%u Failed to transition to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );
            }

            return ok__;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "deinit", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_getMemoryData(CoreFsm_Context* const self, unsigned id, VoidPtrPtr data) {
    switch (self->state) {
        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameLoaded", "getMemoryData"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }


            *data = self->core->getMemoryData(id);
        
            self->state = COREFSM_STATE_GAME_LOADED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "getMemoryData"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            *data = self->core->getMemoryData(id);
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "getMemoryData", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_getMemorySize(CoreFsm_Context* const self, unsigned id, SizePtr size) {
    switch (self->state) {
        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameLoaded", "getMemorySize"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }


            *size = self->core->getMemorySize(id);
        
            self->state = COREFSM_STATE_GAME_LOADED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "getMemorySize"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            *size = self->core->getMemorySize(id);
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "getMemorySize", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_getRegion(CoreFsm_Context* const self, UnsignedPtr region) {
    switch (self->state) {
        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameLoaded", "getRegion"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }


            *region = self->core->getRegion();
        
            self->state = COREFSM_STATE_GAME_LOADED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "getRegion"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            *region = self->core->getRegion();
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "getRegion", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_getSystemAvInfo(CoreFsm_Context* const self, RetroSystemAvInfoPtr info) {
    switch (self->state) {
        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameLoaded", "getSystemAvInfo"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }


            self->core->getSystemAvInfo(info);
        
            self->state = COREFSM_STATE_GAME_LOADED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "getSystemAvInfo"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            self->core->getSystemAvInfo(info);
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "getSystemAvInfo", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_getSystemInfo(CoreFsm_Context* const self, RetroSystemInfoPtr info) {
    switch (self->state) {
        case COREFSM_STATE_CORE_INITIALIZED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreInitialized", "getSystemInfo"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }


            self->core->getSystemInfo(info);
        
            self->state = COREFSM_STATE_CORE_INITIALIZED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_CORE_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreSet", "getSystemInfo"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }


            self->core->getSystemInfo(info);
        
            self->state = COREFSM_STATE_CORE_SET;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreSet"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_ENVIRONMENT_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "EnvironmentSet", "getSystemInfo"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }


            self->core->getSystemInfo(info);
        
            self->state = COREFSM_STATE_ENVIRONMENT_SET;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameLoaded", "getSystemInfo"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }


            self->core->getSystemInfo(info);
        
            self->state = COREFSM_STATE_GAME_LOADED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "getSystemInfo"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            self->core->getSystemInfo(info);
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "getSystemInfo", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_gotoCoreSet(CoreFsm_Context* const self) {
    switch (self->state) {
        case COREFSM_STATE_ENVIRONMENT_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreSet", "gotoCoreSet"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            self->state = COREFSM_STATE_CORE_SET;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreSet"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "gotoCoreSet", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_init(CoreFsm_Context* const self) {
    switch (self->state) {
        case COREFSM_STATE_ENVIRONMENT_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreInitialized", "init"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }


            self->core->init();
        
            self->state = COREFSM_STATE_CORE_INITIALIZED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "init", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_loadGame(CoreFsm_Context* const self, ConstRetroGameInfoPtr gameInfo) {
    switch (self->state) {
        case COREFSM_STATE_CORE_INITIALIZED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameLoaded", "loadGame"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }


            if (!self->core->loadGame(gameInfo)) {
                return 0;
            }
        
            self->state = COREFSM_STATE_GAME_LOADED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "loadGame", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_loadGameSpecial(CoreFsm_Context* const self, unsigned gameType, ConstRetroGameInfoPtr info, size_t numInfo) {
    switch (self->state) {
        case COREFSM_STATE_CORE_INITIALIZED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameLoaded", "loadGameSpecial"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }


            if (!self->core->loadGameSpecial(gameType, info, numInfo)) {
                return 0;
            }
        
            self->state = COREFSM_STATE_GAME_LOADED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "loadGameSpecial", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_reset(CoreFsm_Context* const self) {
    switch (self->state) {
        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "reset"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            self->core->reset();
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "reset", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_run(CoreFsm_Context* const self) {
    switch (self->state) {
        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "run"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            self->core->run();
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "run", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_serialize(CoreFsm_Context* const self, VoidPtr data, size_t size) {
    switch (self->state) {
        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "serialize"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            if (!self->core->serialize(data, size)) {
                return 0;
            }
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "serialize", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_serializeSize(CoreFsm_Context* const self, SizePtr size) {
    switch (self->state) {
        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "serializeSize"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            *size = self->core->serializeSize();
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "serializeSize", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_setCallbacks(CoreFsm_Context* const self, retro_video_refresh_t videoRefresh, retro_audio_sample_t audioSample, retro_audio_sample_batch_t audioSampleBatch, retro_input_poll_t inputPoll, retro_input_state_t inputState) {
    switch (self->state) {
        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "setCallbacks"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            self->core->setVideoRefresh(videoRefresh);
            self->core->setAudioSample(audioSample);
            self->core->setAudioSampleBatch(audioSampleBatch);
            self->core->setInputPoll(inputPoll);
            self->core->setInputState(inputState);
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "setCallbacks", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_setControllerPortDevice(CoreFsm_Context* const self, unsigned port, unsigned device) {
    switch (self->state) {
        case COREFSM_STATE_CORE_INITIALIZED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreInitialized", "setControllerPortDevice"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }


            self->core->setControllerPortDevice(port, device);
        
            self->state = COREFSM_STATE_CORE_INITIALIZED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_ENVIRONMENT_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "EnvironmentSet", "setControllerPortDevice"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }


            self->core->setControllerPortDevice(port, device);
        
            self->state = COREFSM_STATE_ENVIRONMENT_SET;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameLoaded", "setControllerPortDevice"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }


            self->core->setControllerPortDevice(port, device);
        
            self->state = COREFSM_STATE_GAME_LOADED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "setControllerPortDevice"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            self->core->setControllerPortDevice(port, device);
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "setControllerPortDevice", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_setEnvironment(CoreFsm_Context* const self, retro_environment_t cb) {
    switch (self->state) {
        case COREFSM_STATE_CORE_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "EnvironmentSet", "setEnvironment"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }


            self->core->setEnvironment(cb);
        
            self->state = COREFSM_STATE_ENVIRONMENT_SET;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "setEnvironment", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_unloadGame(CoreFsm_Context* const self) {
    switch (self->state) {
        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreInitialized", "unloadGame"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }


            self->core->unloadGame();
        
            self->state = COREFSM_STATE_CORE_INITIALIZED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "CoreInitialized", "unloadGame"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }


            self->core->unloadGame();
        
            self->state = COREFSM_STATE_CORE_INITIALIZED;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "unloadGame", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_unserialize(CoreFsm_Context* const self, ConstVoidPtr data, size_t size) {
    switch (self->state) {
        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "GameRunning", "unserialize"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }


            if (!self->core->unserialize(data, size)) {
                return 0;
            }
        
            self->state = COREFSM_STATE_GAME_RUNNING;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "unserialize", LifeCycle_StateName(self->state)
    );

    return 0;
}

int CoreFsm_Transition_unset(CoreFsm_Context* const self) {
    switch (self->state) {
        case COREFSM_STATE_CORE_INITIALIZED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "Start", "unset"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            const int ok__ = CoreFsm_Transition_deinit(self) &&
                             CoreFsm_Transition_unset(self);

            if (ok__) {
                local_after(self);
                global_after(self);

            }
            else {
                PRINTF(
                    self,
                    "FSM %s:%u Failed to transition to %s",
                    __FILE__, __LINE__, "Start"
                );
            }

            return ok__;
        }

        break;

        case COREFSM_STATE_CORE_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "Start", "unset"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            self->state = COREFSM_STATE_START;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Transitioned to %s",
                __FILE__, __LINE__, "Start"
            );

            return 1;
        }

        break;

        case COREFSM_STATE_ENVIRONMENT_SET: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "Start", "unset"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            const int ok__ = CoreFsm_Transition_gotoCoreSet(self) &&
                             CoreFsm_Transition_unset(self);

            if (ok__) {
                local_after(self);
                global_after(self);

            }
            else {
                PRINTF(
                    self,
                    "FSM %s:%u Failed to transition to %s",
                    __FILE__, __LINE__, "Start"
                );
            }

            return ok__;
        }

        break;

        case COREFSM_STATE_GAME_LOADED: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "Start", "unset"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            const int ok__ = CoreFsm_Transition_unloadGame(self) &&
                             CoreFsm_Transition_unset(self);

            if (ok__) {
                local_after(self);
                global_after(self);

            }
            else {
                PRINTF(
                    self,
                    "FSM %s:%u Failed to transition to %s",
                    __FILE__, __LINE__, "Start"
                );
            }

            return ok__;
        }

        break;

        case COREFSM_STATE_GAME_RUNNING: {
            PRINTF(
                self,
                "FSM %s:%u Transitioning from %s to %s via %s",
                __FILE__, __LINE__, LifeCycle_StateName(LifeCycle_CurrentState(self)), "Start", "unset"
            );

            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while transitioning to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            const int ok__ = CoreFsm_Transition_unloadGame(self) &&
                             CoreFsm_Transition_unset(self);

            if (ok__) {
                local_after(self);
                global_after(self);

            }
            else {
                PRINTF(
                    self,
                    "FSM %s:%u Failed to transition to %s",
                    __FILE__, __LINE__, "Start"
                );
            }

            return ok__;
        }

        break;

        default: break;
    }

    PRINTF(
        self,
        "FSM %s:%u Transition %s is invalid from state %s",
        __FILE__, __LINE__, "unset", LifeCycle_StateName(self->state)
    );

    return 0;
}

#ifdef DEBUG_FSM
const char* CoreFsm_StateName(CoreFsm_State const state) {
    switch (state) {
        case COREFSM_STATE_CORE_INITIALIZED: return "CoreInitialized";
        case COREFSM_STATE_CORE_SET: return "CoreSet";
        case COREFSM_STATE_ENVIRONMENT_SET: return "EnvironmentSet";
        case COREFSM_STATE_GAME_LOADED: return "GameLoaded";
        case COREFSM_STATE_GAME_RUNNING: return "GameRunning";
        case COREFSM_STATE_START: return "Start";
        default: break;
    }

    return "unknown state";
}

const char* CoreFsm_TransitionName(CoreFsm_Transition const transition) {
    switch (transition) {
        case COREFSM_TRANSITION_API_VERSION: return "apiVersion";
        case COREFSM_TRANSITION_CHEAT_RESET: return "cheatReset";
        case COREFSM_TRANSITION_CHEAT_SET: return "cheatSet";
        case COREFSM_TRANSITION_CORE_SET: return "coreSet";
        case COREFSM_TRANSITION_DEINIT: return "deinit";
        case COREFSM_TRANSITION_GET_MEMORY_DATA: return "getMemoryData";
        case COREFSM_TRANSITION_GET_MEMORY_SIZE: return "getMemorySize";
        case COREFSM_TRANSITION_GET_REGION: return "getRegion";
        case COREFSM_TRANSITION_GET_SYSTEM_AV_INFO: return "getSystemAvInfo";
        case COREFSM_TRANSITION_GET_SYSTEM_INFO: return "getSystemInfo";
        case COREFSM_TRANSITION_GOTO_CORE_SET: return "gotoCoreSet";
        case COREFSM_TRANSITION_INIT: return "init";
        case COREFSM_TRANSITION_LOAD_GAME: return "loadGame";
        case COREFSM_TRANSITION_LOAD_GAME_SPECIAL: return "loadGameSpecial";
        case COREFSM_TRANSITION_RESET: return "reset";
        case COREFSM_TRANSITION_RUN: return "run";
        case COREFSM_TRANSITION_SERIALIZE: return "serialize";
        case COREFSM_TRANSITION_SERIALIZE_SIZE: return "serializeSize";
        case COREFSM_TRANSITION_SET_CALLBACKS: return "setCallbacks";
        case COREFSM_TRANSITION_SET_CONTROLLER_PORT_DEVICE: return "setControllerPortDevice";
        case COREFSM_TRANSITION_SET_ENVIRONMENT: return "setEnvironment";
        case COREFSM_TRANSITION_UNLOAD_GAME: return "unloadGame";
        case COREFSM_TRANSITION_UNSERIALIZE: return "unserialize";
        case COREFSM_TRANSITION_UNSET: return "unset";
        default: break;
    }

    return "unknown transition";
}
#endif
