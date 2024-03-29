/* Generated with FSM compiler: https://github.com/leiradel/luamods/tree/master/ddlt */

#include <stddef.h>

/*#line 46 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

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

#define PRINTF(self, ...) do { fsmprintf(self, __VA_ARGS__); } while (0)
#else
#define PRINTF(self, ...) do {} while (0)
#endif

/* Initialization */
void CoreFsm_Init(CoreFsm_Context* const self, CorePtr const core, FrontendPtr const frontend, FrontendPtr const previous) {
    self->state = CoreFsm_State_Start;

    self->core = core;
    self->frontend = frontend;
    self->previous = previous;
}

CoreFsm_State CoreFsm_CurrentState(CoreFsm_Context const* const self) {
    return self->state;
}

int CoreFsm_CanTransitionTo(CoreFsm_Context const* const self, CoreFsm_State const next) {
    switch (self->state) {
        case CoreFsm_State_CoreInitialized:
            switch (next) {
                case CoreFsm_State_CoreInitialized:
                case CoreFsm_State_EnvironmentSet:
                case CoreFsm_State_GameLoaded:
                case CoreFsm_State_Start:
                    return 1;
                default: break;
            }
            break;

        case CoreFsm_State_CoreSet:
            switch (next) {
                case CoreFsm_State_CoreSet:
                case CoreFsm_State_EnvironmentSet:
                case CoreFsm_State_Start:
                    return 1;
                default: break;
            }
            break;

        case CoreFsm_State_EnvironmentSet:
            switch (next) {
                case CoreFsm_State_CoreInitialized:
                case CoreFsm_State_CoreSet:
                case CoreFsm_State_EnvironmentSet:
                case CoreFsm_State_Start:
                    return 1;
                default: break;
            }
            break;

        case CoreFsm_State_GameLoaded:
            switch (next) {
                case CoreFsm_State_CoreInitialized:
                case CoreFsm_State_EnvironmentSet:
                case CoreFsm_State_GameLoaded:
                case CoreFsm_State_GameRunning:
                case CoreFsm_State_Start:
                    return 1;
                default: break;
            }
            break;

        case CoreFsm_State_GameRunning:
            switch (next) {
                case CoreFsm_State_CoreInitialized:
                case CoreFsm_State_EnvironmentSet:
                case CoreFsm_State_GameRunning:
                case CoreFsm_State_Start:
                    return 1;
                default: break;
            }
            break;

        case CoreFsm_State_Start:
            switch (next) {
                case CoreFsm_State_CoreSet:
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
/*#line 52 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

        self->previous = lrcpp::Frontend::getCurrent();
        lrcpp::Frontend::setCurrent(self->frontend);
    
    return 1;
}

static int local_before(CoreFsm_Context* const self) {
    switch (self->state) {
        default: break;
    }

    return 1;
}

static void global_after(CoreFsm_Context* const self) {
    (void)self;
/*#line 57 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

        lrcpp::Frontend::setCurrent(self->previous);
        self->previous = nullptr;
    
}

static void local_after(CoreFsm_Context* const self) {
    switch (self->state) {
        default: break;
    }
}

int CoreFsm_Transition_apiVersion(CoreFsm_Context* const self, UnsignedPtr version) {
    switch (self->state) {
        case CoreFsm_State_CoreInitialized: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

/*#line 105 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *version = self->core->apiVersion();
        
            self->state = CoreFsm_State_CoreInitialized;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        case CoreFsm_State_CoreSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

/*#line 67 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *version = self->core->apiVersion();
        
            self->state = CoreFsm_State_CoreSet;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreSet"
            );

            return 1;
        }

        break;

        case CoreFsm_State_EnvironmentSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

/*#line 83 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *version = self->core->apiVersion();
        
            self->state = CoreFsm_State_EnvironmentSet;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

/*#line 137 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *version = self->core->apiVersion();
        
            self->state = CoreFsm_State_GameLoaded;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 189 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *version = self->core->apiVersion();
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_cheatReset(CoreFsm_Context* const self) {
    switch (self->state) {
        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 229 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->cheatReset();
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_cheatSet(CoreFsm_Context* const self, unsigned index, bool enabled, ConstCharPtr code) {
    switch (self->state) {
        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 233 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->cheatSet(index, enabled, code);
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_coreSet(CoreFsm_Context* const self) {
    switch (self->state) {
        case CoreFsm_State_Start: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            self->state = CoreFsm_State_CoreSet;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreSet"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_deinit(CoreFsm_Context* const self) {
    switch (self->state) {
        case CoreFsm_State_CoreInitialized: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

/*#line 129 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->deinit();
        
            self->state = CoreFsm_State_EnvironmentSet;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
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
                    "FSM %s:%u Failed to switch to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );
            }

            return ok__;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
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
                    "FSM %s:%u Failed to switch to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );
            }

            return ok__;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_getMemoryData(CoreFsm_Context* const self, unsigned id, VoidPtrPtr data) {
    switch (self->state) {
        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

/*#line 175 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *data = self->core->getMemoryData(id);
        
            self->state = CoreFsm_State_GameLoaded;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 241 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *data = self->core->getMemoryData(id);
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_getMemorySize(CoreFsm_Context* const self, unsigned id, SizePtr size) {
    switch (self->state) {
        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

/*#line 179 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *size = self->core->getMemorySize(id);
        
            self->state = CoreFsm_State_GameLoaded;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 245 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *size = self->core->getMemorySize(id);
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_getRegion(CoreFsm_Context* const self, UnsignedPtr region) {
    switch (self->state) {
        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

/*#line 171 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *region = self->core->getRegion();
        
            self->state = CoreFsm_State_GameLoaded;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 237 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *region = self->core->getRegion();
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_getSystemAvInfo(CoreFsm_Context* const self, RetroSystemAvInfoPtr info) {
    switch (self->state) {
        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

/*#line 149 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->getSystemAvInfo(info);
        
            self->state = CoreFsm_State_GameLoaded;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 201 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->getSystemAvInfo(info);
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_getSystemInfo(CoreFsm_Context* const self, RetroSystemInfoPtr info) {
    switch (self->state) {
        case CoreFsm_State_CoreInitialized: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

/*#line 109 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->getSystemInfo(info);
        
            self->state = CoreFsm_State_CoreInitialized;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        case CoreFsm_State_CoreSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

/*#line 71 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->getSystemInfo(info);
        
            self->state = CoreFsm_State_CoreSet;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreSet"
            );

            return 1;
        }

        break;

        case CoreFsm_State_EnvironmentSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

/*#line 87 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->getSystemInfo(info);
        
            self->state = CoreFsm_State_EnvironmentSet;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

/*#line 141 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->getSystemInfo(info);
        
            self->state = CoreFsm_State_GameLoaded;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 193 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->getSystemInfo(info);
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_gotoCoreSet(CoreFsm_Context* const self) {
    switch (self->state) {
        case CoreFsm_State_EnvironmentSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreSet"
                );

                return 0;
            }

            self->state = CoreFsm_State_CoreSet;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreSet"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_init(CoreFsm_Context* const self) {
    switch (self->state) {
        case CoreFsm_State_EnvironmentSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

/*#line 95 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->init();
        
            self->state = CoreFsm_State_CoreInitialized;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_loadGame(CoreFsm_Context* const self, ConstRetroGameInfoPtr gameInfo) {
    switch (self->state) {
        case CoreFsm_State_CoreInitialized: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

/*#line 117 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            if (!self->core->loadGame(gameInfo)) {
                return 0;
            }
        
            self->state = CoreFsm_State_GameLoaded;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_loadGameSpecial(CoreFsm_Context* const self, unsigned gameType, ConstRetroGameInfoPtr info, size_t numInfo) {
    switch (self->state) {
        case CoreFsm_State_CoreInitialized: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

/*#line 123 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            if (!self->core->loadGameSpecial(gameType, info, numInfo)) {
                return 0;
            }
        
            self->state = CoreFsm_State_GameLoaded;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_reset(CoreFsm_Context* const self) {
    switch (self->state) {
        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 209 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->reset();
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_run(CoreFsm_Context* const self) {
    switch (self->state) {
        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 205 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->run();
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_serialize(CoreFsm_Context* const self, VoidPtr data, size_t size) {
    switch (self->state) {
        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 217 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            if (!self->core->serialize(data, size)) {
                return 0;
            }
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_serializeSize(CoreFsm_Context* const self, SizePtr size) {
    switch (self->state) {
        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 213 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            *size = self->core->serializeSize();
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_setCallbacks(CoreFsm_Context* const self, retro_video_refresh_t videoRefresh, retro_audio_sample_t audioSample, retro_audio_sample_batch_t audioSampleBatch, retro_input_poll_t inputPoll, retro_input_state_t inputState) {
    switch (self->state) {
        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 159 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->setVideoRefresh(videoRefresh);
            self->core->setAudioSample(audioSample);
            self->core->setAudioSampleBatch(audioSampleBatch);
            self->core->setInputPoll(inputPoll);
            self->core->setInputState(inputState);
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_setControllerPortDevice(CoreFsm_Context* const self, unsigned port, unsigned device) {
    switch (self->state) {
        case CoreFsm_State_CoreInitialized: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

/*#line 113 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->setControllerPortDevice(port, device);
        
            self->state = CoreFsm_State_CoreInitialized;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        case CoreFsm_State_EnvironmentSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

/*#line 91 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->setControllerPortDevice(port, device);
        
            self->state = CoreFsm_State_EnvironmentSet;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameLoaded"
                );

                return 0;
            }

/*#line 145 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->setControllerPortDevice(port, device);
        
            self->state = CoreFsm_State_GameLoaded;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameLoaded"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 197 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->setControllerPortDevice(port, device);
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_setEnvironment(CoreFsm_Context* const self, retro_environment_t cb) {
    switch (self->state) {
        case CoreFsm_State_CoreSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "EnvironmentSet"
                );

                return 0;
            }

/*#line 75 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->setEnvironment(cb);
        
            self->state = CoreFsm_State_EnvironmentSet;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "EnvironmentSet"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_unloadGame(CoreFsm_Context* const self) {
    switch (self->state) {
        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

/*#line 167 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->unloadGame();
        
            self->state = CoreFsm_State_CoreInitialized;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "CoreInitialized"
                );

                return 0;
            }

/*#line 249 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            self->core->unloadGame();
        
            self->state = CoreFsm_State_CoreInitialized;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "CoreInitialized"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_unserialize(CoreFsm_Context* const self, ConstVoidPtr data, size_t size) {
    switch (self->state) {
        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "GameRunning"
                );

                return 0;
            }

/*#line 223 "/home/leiradel/Develop/retromancer/TinyDungeons/src/retromancer/src/3rdparty/lrcpp/module/etc/CoreFsm.fsm"*/

            if (!self->core->unserialize(data, size)) {
                return 0;
            }
        
            self->state = CoreFsm_State_GameRunning;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "GameRunning"
            );

            return 1;
        }

        break;

        default: break;
    }

    return 0;
}

int CoreFsm_Transition_unset(CoreFsm_Context* const self) {
    switch (self->state) {
        case CoreFsm_State_CoreInitialized: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
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
                    "FSM %s:%u Failed to switch to %s",
                    __FILE__, __LINE__, "Start"
                );
            }

            return ok__;
        }

        break;

        case CoreFsm_State_CoreSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            self->state = CoreFsm_State_Start;

            local_after(self);
            global_after(self);

            PRINTF(
                self,
                "FSM %s:%u Switched to %s",
                __FILE__, __LINE__, "Start"
            );

            return 1;
        }

        break;

        case CoreFsm_State_EnvironmentSet: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
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
                    "FSM %s:%u Failed to switch to %s",
                    __FILE__, __LINE__, "Start"
                );
            }

            return ok__;
        }

        break;

        case CoreFsm_State_GameLoaded: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
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
                    "FSM %s:%u Failed to switch to %s",
                    __FILE__, __LINE__, "Start"
                );
            }

            return ok__;
        }

        break;

        case CoreFsm_State_GameRunning: {
            if (!global_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FILE__, __LINE__, "Start"
                );

                return 0;
            }

            if (!local_before(self)) {
                PRINTF(
                    self,
                    "FSM %s:%u Failed state precondition while switching to %s",
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
                    "FSM %s:%u Failed to switch to %s",
                    __FILE__, __LINE__, "Start"
                );
            }

            return ok__;
        }

        break;

        default: break;
    }

    return 0;
}

#ifdef DEBUG_FSM
const char* CoreFsm_StateName(CoreFsm_State const state) {
    switch (state) {
        case CoreFsm_State_CoreInitialized: return "CoreInitialized";
        case CoreFsm_State_CoreSet: return "CoreSet";
        case CoreFsm_State_EnvironmentSet: return "EnvironmentSet";
        case CoreFsm_State_GameLoaded: return "GameLoaded";
        case CoreFsm_State_GameRunning: return "GameRunning";
        case CoreFsm_State_Start: return "Start";
        default: break;
    }

    return "unknown state";
}
#endif
