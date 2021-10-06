#include <lrcpp/Components.h>

#include <SDL2/SDL.h>

class Logger : public lrcpp::Logger {
public:
    Logger();

    bool init();
    void destroy();

    void setLevel(retro_log_level level);

    // lrcpp::Logger
    virtual void vprintf(retro_log_level level, char const* format, va_list args) override;

protected:
    static void sdlLog(void* userdata, int category, SDL_LogPriority priority, char const* message);
    static SDL_LogPriority levelToPriority(retro_log_level level);

    SDL_LogPriority _priority;
};
