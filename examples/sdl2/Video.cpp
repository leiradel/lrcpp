#include "Video.h"

Video::Video() {
    reset();
}

bool Video::init(lrcpp::Logger* logger) {
    reset();

    _logger = logger;

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
        logger->error("SDL_InitSubSystem(SDL_INIT_VIDEO) failed: %s", SDL_GetError());
        return false;
    }

    _logger->info("Video subsystem initialized");

    _window = SDL_CreateWindow("lrcpp example with SDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               800, 600, SDL_WINDOW_RESIZABLE);

    if (_window == nullptr) {
        logger->error("SDL_CreateWindow() failed: %s", SDL_GetError());
        destroy();
        return false;
    }

    _logger->info("Window created");

    int const numdrivers = SDL_GetNumRenderDrivers();

    if (numdrivers > 0) {
        for (int i = 0; i < numdrivers; i++) {
            SDL_RendererInfo info;

            if (SDL_GetRenderDriverInfo(i, &info) != 0) {
                _logger->error("SDL_GetRenderDriverInfo() failed: %s", SDL_GetError());
            }
            else {
                _logger->info("Render driver %d: %s", i, info.name);

                _logger->info(
                    "    flags:%s%s%s%s",
                    (info.flags & SDL_RENDERER_SOFTWARE) != 0 ? " SDL_RENDERER_SOFTWARE" : "",
                    (info.flags & SDL_RENDERER_ACCELERATED) != 0 ? " SDL_RENDERER_ACCELERATED" : "",
                    (info.flags & SDL_RENDERER_PRESENTVSYNC) != 0 ? " SDL_RENDERER_PRESENTVSYNC" : "",
                    (info.flags & SDL_RENDERER_TARGETTEXTURE) != 0 ? " SDL_RENDERER_TARGETTEXTURE" : ""
                );

                for (Uint32 j = 0; j < info.num_texture_formats; j++) {
                    _logger->info("    texture_formats[%u]: %s", j, SDL_GetPixelFormatName(info.texture_formats[j]));
                }

                _logger->info("    max_texture: %d x %d", info.max_texture_width, info.max_texture_height);
            }
        }
    }

    _renderer = SDL_CreateRenderer(_window, 0, SDL_RENDERER_ACCELERATED);

    if (_renderer == nullptr) {
        logger->error("SDL_CreateRenderer() failed: %s", SDL_GetError());
        destroy();
        return false;
    }

    _logger->info("Renderer created");
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

void Video::clear() {
    if (SDL_RenderClear(_renderer) != 0) {
        _logger->error("SDL_RenderClear() failed: %s", SDL_GetError());
    }
}

void Video::present() {
    int windowWidth = 0, windowHeight = 0;
    SDL_GetWindowSize(_window, &windowWidth, &windowHeight);

    float height = windowHeight;
    float width = height * _aspectRatio;

    if (width > windowWidth) {
        width = windowWidth;
        height = width / _aspectRatio;
    }

    SDL_Rect src;
    src.x = src.y = 0;
    src.w = _usedWidth;
    src.h = _usedHeight;

    SDL_FRect dest;
    dest.x = (windowWidth - width) / 2.0f;
    dest.y = (windowHeight - height) / 2.0f;
    dest.w = width;
    dest.h = height;

    _logger->debug("Rendering %d x %d texture to %f x %f window at (%f, %f)", src.w, src.h, dest.w, dest.h, dest.x, dest.y);

    if (SDL_RenderCopyF(_renderer, _texture, &src, &dest) != 0) {
        _logger->error("SDL_RenderCopyF() failed: %s", SDL_GetError());
    }

    _logger->debug("Presenting rendered framebuffer");
    SDL_RenderPresent(_renderer);
}

bool Video::setRotation(unsigned rotation) {
    (void)rotation;
    _logger->warn("RETRO_ENVIRONMENT_SET_ROTATION not implemented");
    return false;
}

bool Video::getOverscan(bool* overscan) {
    (void)overscan;
    _logger->warn("RETRO_ENVIRONMENT_GET_OVERSCAN not implemented");
    return false;
}

bool Video::getCanDupe(bool* canDupe) {
    *canDupe = true;
    return true;
}

bool Video::showMessage(retro_message const* message) {
    _logger->warn("RETRO_ENVIRONMENT_SET_MESSAGE not implemented (%u, \"%s\")", message->frames, message->msg);
    return true;
}

bool Video::setPixelFormat(retro_pixel_format format) {
    _pixelFormat = format;

    switch (_pixelFormat) {
        case RETRO_PIXEL_FORMAT_0RGB1555: _logger->info("Pixel format set to RETRO_PIXEL_FORMAT_0RGB1555"); break;
        case RETRO_PIXEL_FORMAT_XRGB8888: _logger->info("Pixel format set to RETRO_PIXEL_FORMAT_XRGB8888"); break;
        case RETRO_PIXEL_FORMAT_RGB565: _logger->info("Pixel format set to RETRO_PIXEL_FORMAT_RGB565"); break;

        default: _pixelFormat = RETRO_PIXEL_FORMAT_UNKNOWN; return false;
    }

    return true;
}

bool Video::setHwRender(retro_hw_render_callback* callback) {
    (void)callback;
    _logger->warn("RETRO_ENVIRONMENT_SET_HW_RENDER not implemented");
    return false;
}

bool Video::setFrameTimeCallback(retro_frame_time_callback const* callback) {
    (void)callback;
    _logger->warn("RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK not implemented");
    return false;
}

bool Video::setSystemAvInfo(retro_system_av_info const* info) {
    _coreFps = info->timing.fps;
    _logger->info("Core FPS set to %f", _coreFps);
    return setGeometry(&info->geometry);
}

bool Video::setGeometry(retro_game_geometry const* geometry) {
    _aspectRatio = geometry->aspect_ratio;

    if (_aspectRatio <= 0) {
        _aspectRatio = (float)geometry->base_width / (float)geometry->base_height;
    }

    _logger->info("Core aspect ratio set to %f", _aspectRatio);

    if (_texture != nullptr) {
        if (geometry->max_width <= _textureWidth && geometry->max_height <= _textureHeight) {
            return true;
        }

        SDL_DestroyTexture(_texture);
        _texture = nullptr;
        _textureWidth = _textureHeight = 0;
    }

    SDL_PixelFormatEnum format = SDL_PIXELFORMAT_UNKNOWN;

    switch (_pixelFormat) {
        case RETRO_PIXEL_FORMAT_0RGB1555: format = SDL_PIXELFORMAT_RGB555; break;
        case RETRO_PIXEL_FORMAT_XRGB8888: format = SDL_PIXELFORMAT_RGB888; break;
        case RETRO_PIXEL_FORMAT_RGB565: format = SDL_PIXELFORMAT_RGB565; break;

        default:
            _logger->error("Unknown pixel format, cannot create texture");
            return false;
    }

    _texture = SDL_CreateTexture(_renderer, format, SDL_TEXTUREACCESS_STREAMING, geometry->max_width, geometry->max_height);
    
    if (_texture == nullptr) {
        _logger->error("SDL_CreateTexture() failed: %s", SDL_GetError());
        return false;
    }

    _textureWidth = geometry->max_width;
    _textureHeight = geometry->max_height;

    _logger->info("Texture created with %u x %u pixels", _textureWidth, _textureHeight);
    return true;
}

bool Video::getCurrentSoftwareFramebuffer(retro_framebuffer* framebuffer) {
    (void)framebuffer;
    _logger->warn("RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER not implemented");
    return false;
}

bool Video::getHwRenderInterface(retro_hw_render_interface const** interface) {
    (void)interface;
    _logger->warn("RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE not implemented");
    return false;
}

bool Video::setHwRenderContextNegotiationInterface(retro_hw_render_context_negotiation_interface const* interface) {
    (void)interface;
    _logger->warn("RETRO_ENVIRONMENT_SET_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE not implemented");
    return false;
}

bool Video::setHwSharedContext() {
    _logger->warn("RETRO_ENVIRONMENT_SET_HW_SHARED_CONTEXT not implemented");
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
        _logger->debug("Last frame duplicated");
        return;
    }

    SDL_Rect rect;
    rect.x = rect.y = 0;
    rect.w = _usedWidth = width;
    rect.h = _usedHeight = height;

    void* texturePixels = nullptr;
    int texturePitch = 0;

    _logger->debug("Refreshing %d x %d texture", rect.w, rect.h);

    if (SDL_LockTexture(_texture, &rect, &texturePixels, &texturePitch) != 0) {
        _logger->error("SDL_LockTexture() failed: %s", SDL_GetError());
        return;
    }

    auto source = static_cast<uint8_t const*>(data);
    auto dest = static_cast<uint8_t*>(texturePixels);

    for (unsigned y = 0; y < height; y++) {
        memcpy(dest, source, pitch);
        source += pitch;
        dest += texturePitch;
    }

    SDL_UnlockTexture(_texture);
}

uintptr_t Video::getCurrentFramebuffer() {
    return 0;
}

retro_proc_address_t Video::getProcAddress(char const* symbol) {
    (void)symbol;
    _logger->warn("RETRO_ENVIRONMENT_SET_HW_SHARED_CONTEXT not implemented");
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
    _usedWidth = _usedHeight = 0;
}
