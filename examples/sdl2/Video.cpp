#include "Video.h"

#include <string.h>

Video::Video() {
    reset();
}

bool Video::init(Config* config, lrcpp::Logger* logger) {
    reset();

    _logger = logger;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        logger->error("SDL_InitSubSystem(SDL_INIT_VIDEO) failed: %s\n", SDL_GetError());
        return false;
    }

    _logger->info("Video subsystem initialized\n");

    _window = SDL_CreateWindow("lrcpp example with SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               800, 600, SDL_WINDOW_RESIZABLE);

    if (_window == nullptr) {
        logger->error("SDL_CreateWindow() failed: %s\n", SDL_GetError());
        destroy();
        return false;
    }

    _logger->info("Window created\n");

    int const count = SDL_GetNumRenderDrivers();
    int renderer = -1;
    char const* rendererName = nullptr;
    config->getOption("sdl2lrcpp_video_renderer", &rendererName);

    for (int i = 0; i < count; i++) {
        SDL_RendererInfo info;

        if (SDL_GetRenderDriverInfo(i, &info) != 0) {
            _logger->error("SDL_GetRenderDriverInfo() failed: %s\n", SDL_GetError());
            continue;
        }

        if (rendererName != nullptr && strcmp(rendererName, info.name) == 0) {
            renderer = i;
        }

        _logger->info("Render driver %d: %s\n", i, info.name);
    }

    if (renderer == -1) {
        _logger->info("Using default video renderer\n");
    }

    _renderer = SDL_CreateRenderer(_window, renderer, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (_renderer == nullptr) {
        logger->error("SDL_CreateRenderer() failed: %s\n", SDL_GetError());
        destroy();
        return false;
    }

    _logger->info("Renderer created\n");

    bool smooth = true;
    config->getOption("sdl2lrcpp_video_smooth", &smooth);

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, smooth ? "1" : "0")) {
        _logger->error("SDL_SetHint() failed: %s\n", SDL_GetError());
    }

    return true;
}

void Video::destroy() {
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
    }

    if (_renderer != nullptr) {
        SDL_DestroyRenderer(_renderer);
    }

    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
    }

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    reset();
}

double Video::getCoreFps() const {
    return _coreFps;
}

unsigned Video::bytesPerPixel(retro_pixel_format format) {
    switch (format) {
        case RETRO_PIXEL_FORMAT_XRGB8888: return 4;
        case RETRO_PIXEL_FORMAT_0RGB1555:
        case RETRO_PIXEL_FORMAT_RGB565: return 2;
        default: return 0;
    }
}

Uint32 Video::toSdlPixelFormat(retro_pixel_format format) {
    switch (format) {
        case RETRO_PIXEL_FORMAT_0RGB1555: return SDL_PIXELFORMAT_RGB555;
        case RETRO_PIXEL_FORMAT_XRGB8888: return SDL_PIXELFORMAT_RGB888;
        case RETRO_PIXEL_FORMAT_RGB565: return SDL_PIXELFORMAT_RGB565;
        default: return SDL_PIXELFORMAT_UNKNOWN;
    }
}

bool Video::createTexture(unsigned width, unsigned height, retro_pixel_format format) {
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }

    Uint32 const sdlFormat = toSdlPixelFormat(format);

    if (sdlFormat == SDL_PIXELFORMAT_UNKNOWN) {
        _logger->error("Unknown pixel format, cannot create texture\n");
        return false;
    }

    _texture = SDL_CreateTexture(_renderer, sdlFormat, SDL_TEXTUREACCESS_STREAMING, width, height);

    if (_texture == nullptr) {
        _logger->error("SDL_CreateTexture() failed: %s\n", SDL_GetError());
        return false;
    }

    _textureWidth = width;
    _textureHeight = height;
    _textureFormat = format;

    _logger->info("Texture created with %u x %u pixels, format %s\n", width, height, SDL_GetPixelFormatName(sdlFormat));
    return true;
}

void Video::present() {
    {
        std::lock_guard<std::mutex> lock(_frameMutex);

        if (_hasFrame) {
            Frame const& front = _frames[_frontIndex];

            if (_texture == nullptr || front.width != _textureWidth || front.height != _textureHeight ||
                front.format != _textureFormat) {

                createTexture(front.width, front.height, front.format);
            }

            if (_texture != nullptr) {
                void* pixels = nullptr;
                int texturePitch = 0;

                if (SDL_LockTexture(_texture, nullptr, &pixels, &texturePitch) == 0) {
                    size_t const rowBytes = static_cast<size_t>(front.width) * bytesPerPixel(front.format);
                    uint8_t const* src = front.pixels.data();
                    uint8_t* dst = static_cast<uint8_t*>(pixels);

                    for (unsigned y = 0; y < front.height; y++) {
                        memcpy(dst, src, rowBytes);
                        src += rowBytes;
                        dst += texturePitch;
                    }

                    SDL_UnlockTexture(_texture);
                }
                else {
                    _logger->error("SDL_LockTexture() failed: %s\n", SDL_GetError());
                }

                _drawAspect = front.aspectRatio;
            }

            _hasFrame = false;
        }
    }

    SDL_RenderClear(_renderer);

    if (_texture != nullptr && _drawAspect > 0.0f) {
        int windowWidth = 0, windowHeight = 0;
        SDL_GetWindowSize(_window, &windowWidth, &windowHeight);

        float height = windowHeight;
        float width = height * _drawAspect;

        if (width > windowWidth) {
            width = windowWidth;
            height = width / _drawAspect;
        }

        SDL_Rect src;
        src.x = src.y = 0;
        src.w = _textureWidth;
        src.h = _textureHeight;

        SDL_FRect dest;
        dest.x = (windowWidth - width) / 2.0f;
        dest.y = (windowHeight - height) / 2.0f;
        dest.w = width;
        dest.h = height;

        if (SDL_RenderCopyF(_renderer, _texture, &src, &dest) != 0) {
            _logger->error("SDL_RenderCopyF() failed: %s\n", SDL_GetError());
        }
    }

    SDL_RenderPresent(_renderer);
}

bool Video::setRotation(unsigned rotation) {
    (void)rotation;
    _logger->warn("RETRO_ENVIRONMENT_SET_ROTATION not implemented\n");
    return false;
}

bool Video::getOverscan(bool* overscan) {
    (void)overscan;
    _logger->warn("RETRO_ENVIRONMENT_GET_OVERSCAN not implemented\n");
    return false;
}

bool Video::getCanDupe(bool* canDupe) {
    *canDupe = true;
    return true;
}

bool Video::showMessage(retro_message const* message) {
    _logger->warn("RETRO_ENVIRONMENT_SET_MESSAGE not implemented (%u, \"%s\")\n", message->frames, message->msg);
    return true;
}

bool Video::setPixelFormat(retro_pixel_format format) {
    switch (format) {
        case RETRO_PIXEL_FORMAT_0RGB1555:
        case RETRO_PIXEL_FORMAT_XRGB8888:
        case RETRO_PIXEL_FORMAT_RGB565:
            _pixelFormat = format;
            _logger->info("Pixel format set to %d\n", format);
            return true;

        default:
            _pixelFormat = RETRO_PIXEL_FORMAT_UNKNOWN;
            return false;
    }
}

bool Video::setHwRender(retro_hw_render_callback* callback) {
    (void)callback;
    _logger->warn("RETRO_ENVIRONMENT_SET_HW_RENDER not implemented\n");
    return false;
}

bool Video::setFrameTimeCallback(retro_frame_time_callback const* callback) {
    (void)callback;
    _logger->warn("RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK not implemented\n");
    return false;
}

bool Video::setSystemAvInfo(retro_system_av_info const* info) {
    _coreFps = info->timing.fps;
    _logger->info("Core FPS set to %f\n", _coreFps);
    return setGeometry(&info->geometry);
}

bool Video::setGeometry(retro_game_geometry const* geometry) {
    _aspectRatio = geometry->aspect_ratio;

    if (_aspectRatio <= 0.0f) {
        _aspectRatio = static_cast<float>(geometry->base_width) / static_cast<float>(geometry->base_height);
    }

    _logger->info("Core aspect ratio set to %f\n", _aspectRatio);
    return true;
}

bool Video::getCurrentSoftwareFramebuffer(retro_framebuffer* framebuffer) {
    (void)framebuffer;
    return false;
}

bool Video::getHwRenderInterface(retro_hw_render_interface const** interface) {
    (void)interface;
    _logger->warn("RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE not implemented\n");
    return false;
}

bool Video::setHwRenderContextNegotiationInterface(retro_hw_render_context_negotiation_interface const* interface) {
    (void)interface;
    _logger->warn("RETRO_ENVIRONMENT_SET_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE not implemented\n");
    return false;
}

bool Video::setHwSharedContext() {
    _logger->warn("RETRO_ENVIRONMENT_SET_HW_SHARED_CONTEXT not implemented\n");
    return false;
}

bool Video::getTargetRefreshRate(float* rate) {
    // TODO return the real monitor refresh rate?
    *rate = 60.0f;
    return true;
}

bool Video::getPreferredHwRender(unsigned* preferred) {
    *preferred = RETRO_HW_CONTEXT_NONE;
    return true;
}

void Video::refresh(void const* data, unsigned width, unsigned height, size_t pitch) {
    if (data == nullptr) {
        _logger->debug("Last frame duplicated\n");
        return;
    }

    unsigned const bpp = bytesPerPixel(_pixelFormat);

    if (bpp == 0) {
        _logger->error("Cannot refresh, unknown pixel format\n");
        return;
    }

    Frame& back = _frames[_backIndex];

    size_t const rowBytes = static_cast<size_t>(width) * bpp;
    back.pixels.resize(rowBytes * height);
    back.width = width;
    back.height = height;
    back.format = _pixelFormat;
    back.aspectRatio = _aspectRatio > 0.0f ? _aspectRatio : static_cast<float>(width) / static_cast<float>(height);

    uint8_t const* src = static_cast<uint8_t const*>(data);
    uint8_t* dst = back.pixels.data();

    for (unsigned y = 0; y < height; y++) {
        memcpy(dst, src, rowBytes);
        src += pitch;
        dst += rowBytes;
    }

    std::lock_guard<std::mutex> lock(_frameMutex);
    int const swap = _frontIndex;
    _frontIndex = _backIndex;
    _backIndex = swap;
    _hasFrame = true;
}

uintptr_t Video::getCurrentFramebuffer() {
    return 0;
}

retro_proc_address_t Video::getProcAddress(char const* symbol) {
    (void)symbol;
    _logger->warn("RETRO_ENVIRONMENT_GET_PROC_ADDRESS not implemented\n");
    return nullptr;
}

void Video::reset() {
    _logger = nullptr;

    _window = nullptr;
    _renderer = nullptr;

    _pixelFormat = RETRO_PIXEL_FORMAT_UNKNOWN;
    _coreFps = 0.0;
    _aspectRatio = 0.0f;

    _texture = nullptr;
    _textureWidth = _textureHeight = 0;
    _textureFormat = RETRO_PIXEL_FORMAT_UNKNOWN;
    _drawAspect = 0.0f;

    for (int i = 0; i < 2; i++) {
        _frames[i].pixels.clear();
        _frames[i].width = _frames[i].height = 0;
        _frames[i].format = RETRO_PIXEL_FORMAT_UNKNOWN;
        _frames[i].aspectRatio = 0.0f;
    }

    _backIndex = 0;
    _frontIndex = 1;
    _hasFrame = false;
}
