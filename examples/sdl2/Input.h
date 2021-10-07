#pragma once

#include <lrcpp/Components.h>

#include <SDL2/SDL.h>

#include <string>
#include <unordered_map>
#include <vector>

class Input: public lrcpp::Input {
public:
    Input();

    bool init(lrcpp::Logger* logger);
    void destroy();

    void process(SDL_Event const* event);

    // lrcpp::Input
    virtual bool setInputDescriptors(retro_input_descriptor const* descriptors) override;
    virtual bool setKeyboardCallback(retro_keyboard_callback const* callback) override;
    virtual bool getInputDeviceCapabilities(uint64_t* capabilities) override;
    virtual bool setControllerInfo(retro_controller_info const* info) override;
    virtual bool getInputBitmasks(bool* supports) override;

    virtual int16_t state(unsigned port, unsigned device, unsigned index, unsigned id) override;
    virtual void poll() override;

protected:
    void process(SDL_JoyDeviceEvent const* event);
    void process(SDL_ControllerDeviceEvent const* event);
    void process(SDL_ControllerButtonEvent const* event);
    void process(SDL_ControllerAxisEvent const* event);
    void process(SDL_KeyboardEvent const* event);
    void process(SDL_MouseButtonEvent const* event);
    void process(SDL_MouseMotionEvent const* event);
    void reset();

    static unsigned keycodeToLibretro(SDL_Keycode code);

    struct Gamepad {
        Gamepad();

        struct Axes {
            int16_t x, y;
        };

        Sint32 deviceIndex;
        SDL_JoystickID instanceId;
        SDL_GameController* controller;
        std::string controllerName;
        SDL_Joystick* joystick;
        std::string joystickName;
        int lastDir[6];
        bool state[16];
        Axes analogs[3];
        float sensitivity;
        bool digital;
    };

    lrcpp::Logger* _logger;

    std::unordered_map<Sint32, Gamepad> _gamepads;
    std::vector<Gamepad*> _ports;

    int _mouseX;
    int _mouseY;
    bool _mouseButtons[5];

    retro_keyboard_callback _keyboardCallback;
    bool _keyboardPreviousState[RETROK_LAST];
    bool _keyboardState[RETROK_LAST];
};
