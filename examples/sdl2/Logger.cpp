#include "Logger.h"

Logger::Logger() {}

bool Logger::init() {
    _priority = SDL_LOG_PRIORITY_INFO;

    SDL_LogSetOutputFunction(sdlLog, this);
    return true;
}

void Logger::destroy() {}

void Logger::setLevel(retro_log_level level) {
    _priority = levelToPriority(level);
}

void Logger::vprintf(retro_log_level level, char const* format, va_list args) {
    SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, levelToPriority(level), format, args);
}

void Logger::sdlLog(void* userdata, int category, SDL_LogPriority priority, char const* message) {
    auto self = static_cast<Logger*>(userdata);

    if (priority < self->_priority) {
        return;
    }

    char const* level = nullptr;

    switch (priority) {
        case SDL_LOG_PRIORITY_VERBOSE: level = "VERB "; break;
        case SDL_LOG_PRIORITY_DEBUG: level = "DEBUG"; break;
        case SDL_LOG_PRIORITY_INFO: level = "INFO "; break;
        case SDL_LOG_PRIORITY_WARN: level = "WARN "; break;
        case SDL_LOG_PRIORITY_ERROR: level = "ERROR"; break;
        case SDL_LOG_PRIORITY_CRITICAL: level = "CRIT "; break;

        default: level = "?????"; break;
    }

    fprintf(stderr, "[%s] %s\n", level, message);
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
