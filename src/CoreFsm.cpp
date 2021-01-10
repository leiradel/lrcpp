// Generated with FSM compiler, https://github.com/leiradel/luamods/ddlt

#include "CoreFsm.h"

//#line 44 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

    #include "Core.h"


#ifdef DEBUG_FSM
const char* CoreFsm::stateName(State state) const {
    switch (state) {
        case State::CoreInitialized: return "CoreInitialized";
        case State::CoreLoaded: return "CoreLoaded";
        case State::EnvironmentSet: return "EnvironmentSet";
        case State::GameLoaded: return "GameLoaded";
        case State::GameRunning: return "GameRunning";
        case State::Start: return "Start";
        default: break;
    }

    return NULL;
}

void CoreFsm::printf(const char* fmt, ...) {
    if (__vprintf != nullptr) {
        va_list args;
        va_start(args, fmt);
        __vprintf(__vprintfud, fmt, args);
        va_end(args);
    }
}
#endif

bool CoreFsm::before() const {
    return true;
}

bool CoreFsm::before(State state) const {
    switch (state) {
        default: break;
    }

    return true;
}

void CoreFsm::after() const {
}

void CoreFsm::after(State state) const {
    switch (state) {
        default: break;
    }
}

bool CoreFsm::apiVersion(unsigned_ptr_t version) {
    switch (__state) {
        case State::CoreInitialized: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

//#line 100 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *version = _ctx.apiVersion();
        
            __state = State::CoreInitialized;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
            );
#endif
            return true;
        }
        break;

        case State::CoreLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
                );
#endif

                return false;
            }

//#line 60 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *version = _ctx.apiVersion();
        
            __state = State::CoreLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
            );
#endif
            return true;
        }
        break;

        case State::EnvironmentSet: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

//#line 78 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *version = _ctx.apiVersion();
        
            __state = State::EnvironmentSet;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
            );
#endif
            return true;
        }
        break;

        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

//#line 132 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *version = _ctx.apiVersion();
        
            __state = State::GameLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameLoaded)
            );
#endif
            return true;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 184 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *version = _ctx.apiVersion();
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::cheatReset() {
    switch (__state) {
        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 224 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.cheatReset();
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::cheatSet(unsigned index, bool enabled, const_char_ptr_t code) {
    switch (__state) {
        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 228 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.cheatSet(index, enabled, code);
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::deinit() {
    switch (__state) {
        case State::CoreInitialized: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

//#line 124 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.deinit();
        
            __state = State::EnvironmentSet;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
            );
#endif
            return true;
        }
        break;

        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

            bool __ok = unloadGame() &&
                        deinit();

            if (__ok) {
                after(__state);
                after();

            }
            else {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed to switch to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif
            }

            return __ok;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

            bool __ok = unloadGame() &&
                        deinit();

            if (__ok) {
                after(__state);
                after();

            }
            else {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed to switch to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif
            }

            return __ok;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::getMemoryData(unsigned id, void_ptr_ptr_t data) {
    switch (__state) {
        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

//#line 170 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *data = _ctx.getMemoryData(id);
        
            __state = State::GameLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameLoaded)
            );
#endif
            return true;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 236 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *data = _ctx.getMemoryData(id);
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::getMemorySize(unsigned id, size_ptr_t size) {
    switch (__state) {
        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

//#line 174 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *size = _ctx.getMemorySize(id);
        
            __state = State::GameLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameLoaded)
            );
#endif
            return true;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 240 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *size = _ctx.getMemorySize(id);
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::getRegion(unsigned_ptr_t region) {
    switch (__state) {
        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

//#line 166 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *region = _ctx.getRegion();
        
            __state = State::GameLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameLoaded)
            );
#endif
            return true;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 232 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *region = _ctx.getRegion();
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::getSystemAvInfo(retro_system_av_info_ptr_t info) {
    switch (__state) {
        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

//#line 144 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.getSystemAvInfo(info);
        
            __state = State::GameLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameLoaded)
            );
#endif
            return true;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 196 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.getSystemAvInfo(info);
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::getSystemInfo(retro_system_info_ptr_t info) {
    switch (__state) {
        case State::CoreInitialized: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

//#line 104 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.getSystemInfo(info);
        
            __state = State::CoreInitialized;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
            );
#endif
            return true;
        }
        break;

        case State::CoreLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
                );
#endif

                return false;
            }

//#line 64 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.getSystemInfo(info);
        
            __state = State::CoreLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
            );
#endif
            return true;
        }
        break;

        case State::EnvironmentSet: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

//#line 82 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.getSystemInfo(info);
        
            __state = State::EnvironmentSet;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
            );
#endif
            return true;
        }
        break;

        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

//#line 136 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.getSystemInfo(info);
        
            __state = State::GameLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameLoaded)
            );
#endif
            return true;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 188 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.getSystemInfo(info);
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::gotoCoreLoaded() {
    switch (__state) {
        case State::EnvironmentSet: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
                );
#endif

                return false;
            }

            __state = State::CoreLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::init() {
    switch (__state) {
        case State::EnvironmentSet: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

//#line 90 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.init();
        
            __state = State::CoreInitialized;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::load(const_char_ptr_t path) {
    switch (__state) {
        case State::Start: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
                );
#endif

                return false;
            }

//#line 52 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            if (!_ctx.load(path)) {
                return false;
            }
        
            __state = State::CoreLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreLoaded)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::loadGame(const_retro_game_info_ptr_t gameInfo) {
    switch (__state) {
        case State::CoreInitialized: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

//#line 112 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            if (!_ctx.loadGame(gameInfo)) {
                return false;
            }
        
            __state = State::GameLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameLoaded)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::loadGameSpecial(unsigned gameType, const_retro_game_info_ptr_t info, size_t numInfo) {
    switch (__state) {
        case State::CoreInitialized: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

//#line 118 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            if (!_ctx.loadGameSpecial(gameType, info, numInfo)) {
                return false;
            }
        
            __state = State::GameLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameLoaded)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::reset() {
    switch (__state) {
        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 204 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.reset();
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::run() {
    switch (__state) {
        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 200 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.run();
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::serialize(void_ptr_t data, size_t size) {
    switch (__state) {
        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 212 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            if (!_ctx.serialize(data, size)) {
                return false;
            }
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::serializeSize(size_ptr_t size) {
    switch (__state) {
        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 208 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            *size = _ctx.serializeSize();
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::setCallbacks(retro_video_refresh_t videoRefresh, retro_audio_sample_t audioSample, retro_audio_sample_batch_t audioSampleBatch, retro_input_poll_t inputPoll, retro_input_state_t inputState) {
    switch (__state) {
        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 154 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.setVideoRefresh(videoRefresh);
            _ctx.setAudioSample(audioSample);
            _ctx.setAudioSampleBatch(audioSampleBatch);
            _ctx.setInputPoll(inputPoll);
            _ctx.setInputState(inputState);
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::setControllerPortDevice(unsigned port, unsigned device) {
    switch (__state) {
        case State::CoreInitialized: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

//#line 108 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.setControllerPortDevice(port, device);
        
            __state = State::CoreInitialized;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
            );
#endif
            return true;
        }
        break;

        case State::EnvironmentSet: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

//#line 86 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.setControllerPortDevice(port, device);
        
            __state = State::EnvironmentSet;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
            );
#endif
            return true;
        }
        break;

        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameLoaded)
                );
#endif

                return false;
            }

//#line 140 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.setControllerPortDevice(port, device);
        
            __state = State::GameLoaded;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameLoaded)
            );
#endif
            return true;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 192 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.setControllerPortDevice(port, device);
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::setEnvironment(retro_environment_t cb) {
    switch (__state) {
        case State::CoreLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
                );
#endif

                return false;
            }

//#line 68 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.setEnvironment(cb);
        
            __state = State::EnvironmentSet;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::EnvironmentSet)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::unload() {
    switch (__state) {
        case State::CoreInitialized: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

            bool __ok = deinit() &&
                        unload();

            if (__ok) {
                after(__state);
                after();

            }
            else {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed to switch to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif
            }

            return __ok;
        }
        break;

        case State::CoreLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

//#line 72 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.unload();
        
            __state = State::Start;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::Start)
            );
#endif
            return true;
        }
        break;

        case State::EnvironmentSet: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

            bool __ok = gotoCoreLoaded() &&
                        unload();

            if (__ok) {
                after(__state);
                after();

            }
            else {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed to switch to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif
            }

            return __ok;
        }
        break;

        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

            bool __ok = unloadGame() &&
                        unload();

            if (__ok) {
                after(__state);
                after();

            }
            else {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed to switch to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif
            }

            return __ok;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif

                return false;
            }

            bool __ok = unloadGame() &&
                        unload();

            if (__ok) {
                after(__state);
                after();

            }
            else {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed to switch to %s",
                    __FUNCTION__, __LINE__, stateName(State::Start)
                );
#endif
            }

            return __ok;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::unloadGame() {
    switch (__state) {
        case State::GameLoaded: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

//#line 162 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.unloadGame();
        
            __state = State::CoreInitialized;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
            );
#endif
            return true;
        }
        break;

        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
                );
#endif

                return false;
            }

//#line 244 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            _ctx.unloadGame();
        
            __state = State::CoreInitialized;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::CoreInitialized)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

bool CoreFsm::unserialize(const_void_ptr_t data, size_t size) {
    switch (__state) {
        case State::GameRunning: {
            if (!before()) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed global precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

            if (!before(__state)) {
#ifdef DEBUG_FSM
                printf(
                    "FSM %s:%u Failed state precondition while switching to %s",
                    __FUNCTION__, __LINE__, stateName(State::GameRunning)
                );
#endif

                return false;
            }

//#line 218 "/home/leiradel/Develop/lrcpp/etc/CoreFsm.fsm"

            if (!_ctx.unserialize(data, size)) {
                return false;
            }
        
            __state = State::GameRunning;
            after(__state);
            after();

#ifdef DEBUG_FSM
            printf(
                "FSM %s:%u Switched to %s",
                __FUNCTION__, __LINE__, stateName(State::GameRunning)
            );
#endif
            return true;
        }
        break;

        default: break;
    }

    return false;
}

