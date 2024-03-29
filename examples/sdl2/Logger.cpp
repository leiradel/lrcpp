#include "Logger.h"

Logger::Logger() {}

bool Logger::init() {
    _priority = SDL_LOG_PRIORITY_INFO;
    return true;
}

void Logger::destroy() {}

void Logger::setLevel(retro_log_level level) {
    _priority = levelToPriority(level);
    SDL_LogSetAllPriority(_priority);
}

void Logger::vprintf(retro_log_level level, char const* format, va_list args) {
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, levelToPriority(level), format, args);
}

SDL_LogPriority Logger::levelToPriority(retro_log_level level) {
    switch (level) {
        case RETRO_LOG_DEBUG: return SDL_LOG_PRIORITY_DEBUG;
        case RETRO_LOG_INFO: return SDL_LOG_PRIORITY_INFO;
        case RETRO_LOG_WARN: return SDL_LOG_PRIORITY_WARN;
        case RETRO_LOG_ERROR: return SDL_LOG_PRIORITY_ERROR;

        default: return SDL_LOG_PRIORITY_VERBOSE;
    }
}
