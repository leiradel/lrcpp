#include <lrcpp/Components.h>

#include <SDL.h>

class Logger : public lrcpp::Logger {
public:
    Logger();

    bool init();
    void destroy();

    void setLevel(retro_log_level level);

    // lrcpp::Logger
    virtual void vprintf(retro_log_level level, char const* format, va_list args) override;

protected:
    static SDL_LogPriority levelToPriority(retro_log_level level);

    SDL_LogPriority _priority;
};
