#include <lrcpp/Components.h>

class Logger : public lrcpp::Logger {
public:
    Logger();

    bool init();
    void destroy();

    virtual void vprintf(retro_log_level level, char const* format, va_list args) override;
};
