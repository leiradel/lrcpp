#pragma once

#include "Config.h"

#include <lrcpp/Components.h>

#include <SDL.h>

#include <vector>
#include <mutex>
#include <stdint.h>

class Video : public lrcpp::Video {
public:
    Video();

    bool init(Config* config, lrcpp::Logger* logger);
    void destroy();

    double getCoreFps() const;

    void present();

    void getViewport(int* x, int* y, int* width, int* height);

    // lrcpp::Video
    virtual bool setRotation(unsigned rotation) override;
    virtual bool getOverscan(bool* overscan) override;
    virtual bool getCanDupe(bool* canDupe) override;
    virtual bool showMessage(retro_message const* message) override;
    virtual bool setPixelFormat(retro_pixel_format format) override;
    virtual bool setHwRender(retro_hw_render_callback* callback) override;
    virtual bool setFrameTimeCallback(retro_frame_time_callback const* callback) override;
    virtual bool setSystemAvInfo(retro_system_av_info const* info) override;
    virtual bool setGeometry(retro_game_geometry const* geometry) override;
    virtual bool getCurrentSoftwareFramebuffer(retro_framebuffer* framebuffer) override;
    virtual bool getHwRenderInterface(retro_hw_render_interface const** interface) override;
    virtual bool setHwRenderContextNegotiationInterface(retro_hw_render_context_negotiation_interface const* interface) override;
    virtual bool setHwSharedContext() override;
    virtual bool getTargetRefreshRate(float* rate) override;
    virtual bool getPreferredHwRender(unsigned* preferred) override;

    virtual void refresh(void const* data, unsigned width, unsigned height, size_t pitch) override;

    virtual uintptr_t getCurrentFramebuffer() override;
    virtual retro_proc_address_t getProcAddress(char const* symbol) override;

protected:
    void reset();
    bool createTexture(unsigned width, unsigned height, retro_pixel_format format);

    static unsigned bytesPerPixel(retro_pixel_format format);
    static Uint32 toSdlPixelFormat(retro_pixel_format format);

    lrcpp::Logger* _logger;

    SDL_Window* _window;
    SDL_Renderer* _renderer;

    retro_pixel_format _pixelFormat;
    double _coreFps;
    float _aspectRatio;

    SDL_Texture* _texture;
    unsigned _textureWidth;
    unsigned _textureHeight;
    retro_pixel_format _textureFormat;
    float _drawAspect;

    struct Frame {
        std::vector<uint8_t> pixels;
        unsigned width;
        unsigned height;
        retro_pixel_format format;
        float aspectRatio;
    };

    Frame _frames[2];
    int _backIndex;
    int _frontIndex;
    bool _hasFrame;
    std::mutex _frameMutex;

    int _viewportX;
    int _viewportY;
    int _viewportWidth;
    int _viewportHeight;
    std::mutex _viewportMutex;
};
