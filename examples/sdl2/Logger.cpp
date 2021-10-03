#include "Logger.h"

#include <SDL2/SDL.h>

Logger::Logger() {}

bool Logger::init() {
    return true;
}

void Logger::destroy() {}

void Logger::vprintf(retro_log_level level, char const* format, va_list args) {
    SDL_LogPriority priority = SDL_LOG_PRIORITY_DEBUG;

    switch (level) {
        case RETRO_LOG_DUMMY: // fallthrough
        case RETRO_LOG_DEBUG: priority = SDL_LOG_PRIORITY_DEBUG; break;
        case RETRO_LOG_INFO: priority = SDL_LOG_PRIORITY_INFO; break;
        case RETRO_LOG_WARN: priority = SDL_LOG_PRIORITY_WARN; break;
        case RETRO_LOG_ERROR: priority = SDL_LOG_PRIORITY_ERROR; break;
    }

    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, priority, format, args);
}
