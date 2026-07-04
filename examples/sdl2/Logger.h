#pragma once

#include <lrcpp/Components.h>

#include <stdio.h>

class Logger : public lrcpp::Logger {
public:
    Logger();

    bool init();
    void destroy();

    bool setLogPath(char const* path);
    void setLevel(retro_log_level level);

    // lrcpp::Logger
    virtual void vprintf(retro_log_level level, char const* format, va_list args) override;

protected:
    retro_log_level _level;
    FILE* _file;
};
