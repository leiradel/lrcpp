#include "Logger.h"

Logger::Logger() {
    _level = RETRO_LOG_WARN;
    _file = nullptr;
}

bool Logger::init() {
    _level = RETRO_LOG_WARN;
    _file = nullptr;
    return true;
}

void Logger::destroy() {
    if (_file != nullptr) {
        fclose(_file);
        _file = nullptr;
    }
}

bool Logger::setLogPath(char const* path) {
    if (_file != nullptr) {
        fclose(_file);
    }

    _file = fopen(path, "w");
    return _file != nullptr;
}

void Logger::setLevel(retro_log_level level) {
    _level = level;
}

void Logger::vprintf(retro_log_level level, char const* format, va_list args) {
    if (_file == nullptr || level < _level) {
        return;
    }

    char const* tag = "?????";

    switch (level) {
        case RETRO_LOG_DEBUG: tag = "DEBUG"; break;
        case RETRO_LOG_INFO:  tag = "INFO "; break;
        case RETRO_LOG_WARN:  tag = "WARN "; break;
        case RETRO_LOG_ERROR: tag = "ERROR"; break;
        default: break;
    }

    std::lock_guard<std::mutex> lock(_mutex);

    fprintf(_file, "[%s] ", tag);
    vfprintf(_file, format, args);
    fflush(_file);
}
