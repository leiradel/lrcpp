#include "Input.h"

Input::Input() {
    reset();
}

bool Input::init(lrcpp::Logger* logger) {
    reset();

    _logger = logger;

    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) {
        logger->error("SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) failed: %s", SDL_GetError());
        return false;
    }

    _logger->info("Audio subsystem initialized");
    return true;
}

void Input::destroy() {
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
    reset();
}

void Input::process(SDL_Event const* event) {
    switch (event->type) {
        case SDL_JOYDEVICEADDED:
            process(&event->jdevice);
            break;

        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
            process(&event->cdevice);
            break;

        case SDL_CONTROLLERBUTTONUP:
        case SDL_CONTROLLERBUTTONDOWN:
            process(&event->cbutton);
            break;

        case SDL_CONTROLLERAXISMOTION:
            process(&event->caxis);
            break;

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            process(&event->key);
            break;

        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
            process(&event->button);
            break;

        case SDL_MOUSEMOTION:
            process(&event->motion);
            break;
    }
}

bool Input::setInputDescriptors(retro_input_descriptor const* descriptors) {
    _logger->info("Setting input descriptors");
    _logger->info("    port device index id description");

    for (size_t i = 0; descriptors[i].description != nullptr; i++) {
        /**
         * At least the Frodo core doesn't properly terminate the input
         * descriptor list with a zeroed entry, we do our best to avoid a crash
         * here.
         */
        if ((descriptors[i].device & RETRO_DEVICE_MASK) > RETRO_DEVICE_POINTER) {
            break;
        }

        if (descriptors[i].id > RETRO_DEVICE_ID_LIGHTGUN_RELOAD) {
            break;
        }

        retro_input_descriptor const* desc = descriptors + i;
        _logger->info("    %4u %6u %5u %2u %s", desc->port, desc->device, desc->index, desc->id, desc->description);
    }

    return true;
}

bool Input::setKeyboardCallback(retro_keyboard_callback const* callback) {
    _keyboardCallback = *callback;
    return true;
}

bool Input::getInputDeviceCapabilities(uint64_t* capabilities) {
    *capabilities = 1 << RETRO_DEVICE_JOYPAD | 1 << RETRO_DEVICE_MOUSE | 1 << RETRO_DEVICE_KEYBOARD | 1 << RETRO_DEVICE_ANALOG;
    return true;
}

bool Input::setControllerInfo(retro_controller_info const* info) {
    static char const* const deviceNames[] = {"none", "joypad", "mouse", "keyboard", "lightgun", "analog", "pointer"};

    _logger->info("Setting controller info");
    _logger->info("    port id type     description");

    for (size_t i = 0; info[i].types != nullptr; i++) {
        for (unsigned j = 0; j < info[i].num_types; j++) {
            retro_controller_description const* type = info[i].types + j;

            unsigned const deviceType = type->id & RETRO_DEVICE_MASK;
            char const* deviceName = deviceType < sizeof(deviceNames) / sizeof(deviceNames[0]) ? deviceNames[deviceType] : "?";

            _logger->info("    %4zu %2u %-8s %s", i + 1, type->id >> RETRO_DEVICE_TYPE_SHIFT, deviceName, type->desc);
        }
    }

    return true;
}

bool Input::getInputBitmasks(bool* supports) {
    *supports = false;
    return false;
}

int16_t Input::state(unsigned port, unsigned device, unsigned index, unsigned id) {
    unsigned const base = device & RETRO_DEVICE_MASK;

    switch (base) {
        case RETRO_DEVICE_JOYPAD:
        case RETRO_DEVICE_ANALOG: {
            if (port >= _ports.size()) {
                return 0;
            }

            Gamepad* const gamepad = _ports[port];

            if (base == RETRO_DEVICE_JOYPAD) {
                return gamepad->state[id];
            }
            else {
                return id == RETRO_DEVICE_ID_ANALOG_X ? gamepad->analogs[index].x : gamepad->analogs[index].y;
            }

            break;
        }

        case RETRO_DEVICE_KEYBOARD: return _keyboardState[id] ? 32767 : 0;

        case RETRO_DEVICE_MOUSE: {
            switch (id) {
                case RETRO_DEVICE_ID_MOUSE_X: return _mouseX;
                case RETRO_DEVICE_ID_MOUSE_Y: return _mouseY;
                case RETRO_DEVICE_ID_MOUSE_LEFT: return _mouseButtons[0] ? 32767 : 0;
                case RETRO_DEVICE_ID_MOUSE_MIDDLE: return _mouseButtons[1] ? 32767 : 0;
                case RETRO_DEVICE_ID_MOUSE_RIGHT: return _mouseButtons[2] ? 32767 : 0;
                case RETRO_DEVICE_ID_MOUSE_BUTTON_4: return _mouseButtons[3] ? 32767 : 0;
                case RETRO_DEVICE_ID_MOUSE_BUTTON_5: return _mouseButtons[4] ? 32767 : 0;
            }

            break;
        }
    }

    return 0;
}

void Input::poll() {
    if (_keyboardCallback.callback != nullptr) {
        for (int i = RETROK_FIRST; i < RETROK_LAST; i++) {
            if (_keyboardState[i] != _keyboardPreviousState[i]) {
                _keyboardCallback.callback(_keyboardState[i], i, 0, 0);
                _keyboardPreviousState[i] = _keyboardState[i];
            }
        }
    }
}

void Input::process(SDL_JoyDeviceEvent const* event) {
    if (event->type != SDL_JOYDEVICEADDED) {
        return;
    }

    SDL_JoystickGUID const guid = SDL_JoystickGetDeviceGUID(event->which);
    char const* const mapping = SDL_GameControllerMappingForGUID(guid);
    char const* const name = SDL_JoystickNameForIndex(event->which);

    if (mapping == nullptr) {
        char guidStr[128];
        SDL_JoystickGetGUIDString(guid, guidStr, sizeof(guidStr));
        _logger->error("No mapping for joystick \"%s\" (GUID %s), joystick unusable", name, guidStr);
    }
    else {
        SDL_free((void*)mapping);
    }
}

void Input::process(SDL_ControllerDeviceEvent const* event) {
    if (!SDL_IsGameController(event->which)) {
        _logger->warn("SDL device %d is not a controller", event->which);
        return;
    }

    if (event->type == SDL_CONTROLLERDEVICEADDED) {
        SDL_GameController* const controller = SDL_GameControllerOpen(event->which);

        if (controller == nullptr) {
            _logger->error("SDL_GameControllerOpen() failed: %s", SDL_GetError());
            return;
        }

        SDL_Joystick* const joystick = SDL_GameControllerGetJoystick(controller);

        if (joystick == nullptr) {
            _logger->error("SDL_GameControllerGetJoystick() failed: %s", SDL_GetError());
            SDL_GameControllerClose(controller);
            return;
        }

        auto inserted = _gamepads.insert(std::make_pair(event->which, Gamepad()));
        Gamepad* const gamepad = &inserted.first->second;

        gamepad->controller = controller;
        gamepad->joystick = joystick;
        gamepad->deviceIndex = event->which;
        gamepad->instanceId = SDL_JoystickInstanceID(joystick);
        gamepad->controllerName = SDL_GameControllerName(controller);
        gamepad->joystickName = SDL_JoystickName(joystick);

        _ports.emplace_back(gamepad);
        _logger->info("Controller %s (%s) added", gamepad->controllerName.c_str(), gamepad->joystickName.c_str());

        size_t const count = _ports.size();

        for (size_t i = 0; i < count; i++) {
            Gamepad const* const gamepad = _ports[i];
            _logger->info("    Port %zu has controller %s (%s)", i + 1, gamepad->controllerName.c_str(), gamepad->joystickName.c_str());
        }
    }
    else if (event->type == SDL_CONTROLLERDEVICEREMOVED) {
        auto found = _gamepads.find(event->which);

        if (found == _gamepads.end()) {
            return;
        }

        Gamepad* const gamepad = &found->second;
        SDL_GameControllerClose(gamepad->controller);

        for (auto it = _ports.begin(); it != _ports.end(); ++it) {
            if (*it == gamepad) {
                _ports.erase(it);
                break;
            }
        }

        _gamepads.erase(found);
        _logger->info("Controller %s (%s) removed", gamepad->controllerName.c_str(), gamepad->joystickName.c_str());

        size_t const count = _ports.size();

        for (size_t i = 0; i < count; i++) {
            Gamepad const* const gamepad = _ports[i];
            _logger->info("    Port %zu has controller %s (%s)", i + 1, gamepad->controllerName.c_str(), gamepad->joystickName.c_str());
        }
    }
}

void Input::process(SDL_ControllerButtonEvent const* event) {
    auto found = _gamepads.find(event->which);

    if (found == _gamepads.end()) {
        return;
    }

    Gamepad* const gamepad = &found->second;
    unsigned button = 0;

    switch (event->button) {
        case SDL_CONTROLLER_BUTTON_A: button = RETRO_DEVICE_ID_JOYPAD_B; break;
        case SDL_CONTROLLER_BUTTON_B: button = RETRO_DEVICE_ID_JOYPAD_A; break;
        case SDL_CONTROLLER_BUTTON_X: button = RETRO_DEVICE_ID_JOYPAD_Y; break;
        case SDL_CONTROLLER_BUTTON_Y: button = RETRO_DEVICE_ID_JOYPAD_X; break;
        case SDL_CONTROLLER_BUTTON_BACK: button = RETRO_DEVICE_ID_JOYPAD_SELECT; break;
        case SDL_CONTROLLER_BUTTON_START: button = RETRO_DEVICE_ID_JOYPAD_START; break;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK: button = RETRO_DEVICE_ID_JOYPAD_L3; break;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK: button = RETRO_DEVICE_ID_JOYPAD_R3; break;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: button = RETRO_DEVICE_ID_JOYPAD_L; break;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: button = RETRO_DEVICE_ID_JOYPAD_R; break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP: button = RETRO_DEVICE_ID_JOYPAD_UP; break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: button = RETRO_DEVICE_ID_JOYPAD_DOWN; break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: button = RETRO_DEVICE_ID_JOYPAD_LEFT; break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: button = RETRO_DEVICE_ID_JOYPAD_RIGHT; break;
        case SDL_CONTROLLER_BUTTON_GUIDE: // fallthrough
        default: return;
    }

    gamepad->state[button] = event->state == SDL_PRESSED;
}

void Input::process(SDL_ControllerAxisEvent const* event) {
    auto found = _gamepads.find(event->which);

    if (found == _gamepads.end()) {
        return;
    }

    Gamepad* const gamepad = &found->second;

    int const threshold = 32767 * (1.0f - gamepad->sensitivity);
    int positive = 0, negative = 0;
    int button = 0;
    int* lastDir = nullptr;

    switch (event->axis) {
        case SDL_CONTROLLER_AXIS_LEFTX:
        case SDL_CONTROLLER_AXIS_LEFTY:
        case SDL_CONTROLLER_AXIS_RIGHTX:
        case SDL_CONTROLLER_AXIS_RIGHTY:
            switch (event->axis) {
                case SDL_CONTROLLER_AXIS_LEFTX:
                    gamepad->analogs[RETRO_DEVICE_INDEX_ANALOG_LEFT].x = event->value;
                    positive = RETRO_DEVICE_ID_JOYPAD_RIGHT;
                    negative = RETRO_DEVICE_ID_JOYPAD_LEFT;
                    lastDir = gamepad->lastDir + 0;
                    break;

                case SDL_CONTROLLER_AXIS_LEFTY:
                    gamepad->analogs[RETRO_DEVICE_INDEX_ANALOG_LEFT].y = event->value;
                    positive = RETRO_DEVICE_ID_JOYPAD_DOWN;
                    negative = RETRO_DEVICE_ID_JOYPAD_UP;
                    lastDir = gamepad->lastDir + 1;
                    break;

                case SDL_CONTROLLER_AXIS_RIGHTX:
                    gamepad->analogs[RETRO_DEVICE_INDEX_ANALOG_RIGHT].x = event->value;
                    positive = RETRO_DEVICE_ID_JOYPAD_RIGHT;
                    negative = RETRO_DEVICE_ID_JOYPAD_LEFT;
                    lastDir = gamepad->lastDir + 2;
                    break;

                case SDL_CONTROLLER_AXIS_RIGHTY:
                    gamepad->analogs[RETRO_DEVICE_INDEX_ANALOG_RIGHT].y = event->value;
                    positive = RETRO_DEVICE_ID_JOYPAD_DOWN;
                    negative = RETRO_DEVICE_ID_JOYPAD_UP;
                    lastDir = gamepad->lastDir + 3;
                    break;
            }

            if (event->value < -threshold) {
                button = negative;
            }
            else if (event->value > threshold) {
                button = positive;
            }
            else {
                button = -1;
            }

            break;

        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
            if (event->axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
                gamepad->analogs[RETRO_DEVICE_INDEX_ANALOG_BUTTON].x = event->value;
                button = RETRO_DEVICE_ID_JOYPAD_L2;
                lastDir = gamepad->lastDir + 4;
            }
            else {
                gamepad->analogs[RETRO_DEVICE_INDEX_ANALOG_BUTTON].y = event->value;
                button = RETRO_DEVICE_ID_JOYPAD_R2;
                lastDir = gamepad->lastDir + 5;
            }

            break;

        default:
            return;
    }

    if (gamepad->digital) {
        if (*lastDir != -1) {
            gamepad->state[*lastDir] = false;
        }

        if (event->value < -threshold || event->value > threshold) {
            gamepad->state[button] = true;
        }

        *lastDir = button;
    }
}

void Input::process(SDL_KeyboardEvent const* event) {
    if (event->repeat) {
        return;
    }

    unsigned const key = keycodeToLibretro(event->keysym.sym);

    if (key != RETROK_UNKNOWN) {
        _keyboardState[key] = event->state == SDL_PRESSED;
    }
}

void Input::process(SDL_MouseButtonEvent const* event) {
    switch (event->button) {
        case SDL_BUTTON_LEFT:
            _mouseButtons[0] = event->state == SDL_PRESSED;
            break;

        case SDL_BUTTON_MIDDLE:
            _mouseButtons[1] = event->state == SDL_PRESSED;
            break;

        case SDL_BUTTON_RIGHT:
            _mouseButtons[2] = event->state == SDL_PRESSED;
            break;

        case SDL_BUTTON_X1:
            _mouseButtons[3] = event->state == SDL_PRESSED;
            break;

        case SDL_BUTTON_X2:
            _mouseButtons[4] = event->state == SDL_PRESSED;
            break;
    }
}

void Input::process(SDL_MouseMotionEvent const* event) {
    _mouseX += event->xrel;
    _mouseY += event->yrel;
}

void Input::reset() {
    _logger = nullptr;

    _gamepads.clear();
    _ports.clear();

    _mouseX = _mouseY = 0;
    memset(_mouseButtons, 0, sizeof(_mouseButtons));

    _keyboardCallback.callback = nullptr;
    memset(_keyboardPreviousState, 0, sizeof(_keyboardPreviousState));
    memset(_keyboardState, 0, sizeof(_keyboardState));
}

unsigned Input::keycodeToLibretro(SDL_Keycode code) {
    switch (code) {
        case SDLK_RETURN: return RETROK_RETURN;
        case SDLK_ESCAPE: return RETROK_ESCAPE;
        case SDLK_BACKSPACE: return RETROK_BACKSPACE;
        case SDLK_TAB: return RETROK_TAB;
        case SDLK_SPACE: return RETROK_SPACE;
        case SDLK_EXCLAIM: return RETROK_EXCLAIM;
        case SDLK_QUOTEDBL: return RETROK_QUOTEDBL;
        case SDLK_HASH: return RETROK_HASH;
        case SDLK_DOLLAR: return RETROK_DOLLAR;
        case SDLK_AMPERSAND: return RETROK_AMPERSAND;
        case SDLK_QUOTE: return RETROK_QUOTE;
        case SDLK_LEFTPAREN: return RETROK_LEFTPAREN;
        case SDLK_RIGHTPAREN: return RETROK_RIGHTPAREN;
        case SDLK_ASTERISK: return RETROK_ASTERISK;
        case SDLK_PLUS: return RETROK_PLUS;
        case SDLK_COMMA: return RETROK_COMMA;
        case SDLK_MINUS: return RETROK_MINUS;
        case SDLK_PERIOD: return RETROK_PERIOD;
        case SDLK_SLASH: return RETROK_SLASH;
        case SDLK_0: return RETROK_0;
        case SDLK_1: return RETROK_1;
        case SDLK_2: return RETROK_2;
        case SDLK_3: return RETROK_3;
        case SDLK_4: return RETROK_4;
        case SDLK_5: return RETROK_5;
        case SDLK_6: return RETROK_6;
        case SDLK_7: return RETROK_7;
        case SDLK_8: return RETROK_8;
        case SDLK_9: return RETROK_9;
        case SDLK_COLON: return RETROK_COLON;
        case SDLK_SEMICOLON: return RETROK_SEMICOLON;
        case SDLK_LESS: return RETROK_LESS;
        case SDLK_EQUALS: return RETROK_EQUALS;
        case SDLK_GREATER: return RETROK_GREATER;
        case SDLK_QUESTION: return RETROK_QUESTION;
        case SDLK_AT: return RETROK_AT;
        case SDLK_LEFTBRACKET: return RETROK_LEFTBRACKET;
        case SDLK_BACKSLASH: return RETROK_BACKSLASH;
        case SDLK_RIGHTBRACKET: return RETROK_RIGHTBRACKET;
        case SDLK_CARET: return RETROK_CARET;
        case SDLK_UNDERSCORE: return RETROK_UNDERSCORE;
        case SDLK_BACKQUOTE: return RETROK_BACKQUOTE;
        case SDLK_a: return RETROK_a;
        case SDLK_b: return RETROK_b;
        case SDLK_c: return RETROK_c;
        case SDLK_d: return RETROK_d;
        case SDLK_e: return RETROK_e;
        case SDLK_f: return RETROK_f;
        case SDLK_g: return RETROK_g;
        case SDLK_h: return RETROK_h;
        case SDLK_i: return RETROK_i;
        case SDLK_j: return RETROK_j;
        case SDLK_k: return RETROK_k;
        case SDLK_l: return RETROK_l;
        case SDLK_m: return RETROK_m;
        case SDLK_n: return RETROK_n;
        case SDLK_o: return RETROK_o;
        case SDLK_p: return RETROK_p;
        case SDLK_q: return RETROK_q;
        case SDLK_r: return RETROK_r;
        case SDLK_s: return RETROK_s;
        case SDLK_t: return RETROK_t;
        case SDLK_u: return RETROK_u;
        case SDLK_v: return RETROK_v;
        case SDLK_w: return RETROK_w;
        case SDLK_x: return RETROK_x;
        case SDLK_y: return RETROK_y;
        case SDLK_z: return RETROK_z;
        case SDLK_CAPSLOCK: return RETROK_CAPSLOCK;
        case SDLK_F1: return RETROK_F1;
        case SDLK_F2: return RETROK_F2;
        case SDLK_F3: return RETROK_F3;
        case SDLK_F4: return RETROK_F4;
        case SDLK_F5: return RETROK_F5;
        case SDLK_F6: return RETROK_F6;
        case SDLK_F7: return RETROK_F7;
        case SDLK_F8: return RETROK_F8;
        case SDLK_F9: return RETROK_F9;
        case SDLK_F10: return RETROK_F10;
        case SDLK_F11: return RETROK_F11;
        case SDLK_F12: return RETROK_F12;
        case SDLK_PRINTSCREEN: return RETROK_PRINT;
        case SDLK_SCROLLLOCK: return RETROK_SCROLLOCK;
        case SDLK_PAUSE: return RETROK_PAUSE;
        case SDLK_INSERT: return RETROK_INSERT;
        case SDLK_HOME: return RETROK_HOME;
        case SDLK_PAGEUP: return RETROK_PAGEUP;
        case SDLK_DELETE: return RETROK_DELETE;
        case SDLK_END: return RETROK_END;
        case SDLK_PAGEDOWN: return RETROK_PAGEDOWN;
        case SDLK_RIGHT: return RETROK_RIGHT;
        case SDLK_LEFT: return RETROK_LEFT;
        case SDLK_DOWN: return RETROK_DOWN;
        case SDLK_UP: return RETROK_UP;
        case SDLK_NUMLOCKCLEAR: return RETROK_NUMLOCK;
        case SDLK_KP_DIVIDE: return RETROK_KP_DIVIDE;
        case SDLK_KP_MULTIPLY: return RETROK_KP_MULTIPLY;
        case SDLK_KP_MINUS: return RETROK_KP_MINUS;
        case SDLK_KP_PLUS: return RETROK_KP_PLUS;
        case SDLK_KP_ENTER: return RETROK_KP_ENTER;
        case SDLK_KP_1: return RETROK_KP1;
        case SDLK_KP_2: return RETROK_KP2;
        case SDLK_KP_3: return RETROK_KP3;
        case SDLK_KP_4: return RETROK_KP4;
        case SDLK_KP_5: return RETROK_KP5;
        case SDLK_KP_6: return RETROK_KP6;
        case SDLK_KP_7: return RETROK_KP7;
        case SDLK_KP_8: return RETROK_KP8;
        case SDLK_KP_9: return RETROK_KP9;
        case SDLK_KP_0: return RETROK_KP0;
        case SDLK_KP_PERIOD: return RETROK_KP_PERIOD;
        case SDLK_APPLICATION: return RETROK_COMPOSE;
        case SDLK_POWER: return RETROK_POWER;
        case SDLK_KP_EQUALS: return RETROK_KP_EQUALS;
        case SDLK_F13: return RETROK_F13;
        case SDLK_F14: return RETROK_F14;
        case SDLK_F15: return RETROK_F15;
        case SDLK_HELP: return RETROK_HELP;
        case SDLK_MENU: return RETROK_MENU;
        case SDLK_UNDO: return RETROK_UNDO;
        case SDLK_SYSREQ: return RETROK_SYSREQ;
        case SDLK_LCTRL: return RETROK_LCTRL;
        case SDLK_LSHIFT: return RETROK_LSHIFT;
        case SDLK_LALT: return RETROK_LALT;
        case SDLK_LGUI: return RETROK_LMETA;
        case SDLK_RCTRL: return RETROK_RCTRL;
        case SDLK_RSHIFT: return RETROK_RSHIFT;
        case SDLK_RALT: return RETROK_RALT;
        case SDLK_RGUI: return RETROK_RMETA;
        case SDLK_MODE: return RETROK_MODE;

        default: return RETROK_UNKNOWN;
    }
}

Input::Gamepad::Gamepad() {
    deviceIndex = 0;
    instanceId = 0;
    controller = nullptr;
    joystick = nullptr;

    lastDir[0] = 0; lastDir[1] = 0; lastDir[2] = 0;
    lastDir[3] = 0; lastDir[4] = 0; lastDir[5] = 0;

    state[0] = false; state[1] = false; state[2] = false; state[3] = false;
    state[4] = false; state[5] = false; state[6] = false; state[7] = false;
    state[8] = false; state[9] = false; state[10] = false; state[11] = false;
    state[12] = false; state[13] = false; state[14] = false; state[15] = false;

    analogs[0].x = 0; analogs[0].y = 0;
    analogs[1].x = 0; analogs[1].y = 0;
    analogs[2].x = 0; analogs[2].y = 0;

    sensitivity = 0.5f;
    digital = false;
}
