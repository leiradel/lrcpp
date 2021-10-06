#include "Config.h"
#include "Logger.h"
#include "Video.h"

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <string>

static void Usage(FILE* out) {
    fprintf(out, "Usage: sdl2lrcpp [options...] <content path>\n\n");
    fprintf(out, "-L, --libretro   The path to the libretro core\n");
    fprintf(out, "-c, --config     The path to the configuration file (optional)\n");
    fprintf(out, "-v, --verbose    Increase the verboseness one level (up to two levels)\n");
    fprintf(out, "-h, --help       Shows this help screen and exit\n");
    fprintf(out, "<content path>   The path to the content file to use with the core\n");
}

int main(int argc, char const* argv[]) {
    char const* corePathArg = nullptr;
    char const* configPathArg = nullptr;
    char const* contentPathArg = nullptr;
    retro_log_level level = RETRO_LOG_WARN;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--libretro") == 0) {
            if (++i == argc) {
                fprintf(stderr, "Error: missing core path after --libretro\n");
                return EXIT_FAILURE;
            }

            if (corePathArg != nullptr) {
                fprintf(stderr, "Error: core path already specified\n");
                return EXIT_FAILURE;
            }

            corePathArg = argv[i];
        }
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) {
            if (++i == argc) {
                fprintf(stderr, "Error: missing configuration path after --config\n");
                return EXIT_FAILURE;
            }

            if (configPathArg != nullptr) {
                fprintf(stderr, "Error: configuration path already specified\n");
                return EXIT_FAILURE;
            }

            configPathArg = argv[i];
        }
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0) {
            if (level == RETRO_LOG_WARN) {
                level = RETRO_LOG_INFO;
            }
            else if (level == RETRO_LOG_INFO) {
                level = RETRO_LOG_DEBUG;
            }
            else {
                fprintf(stderr, "Error: cannot be more verbose\n");
                return EXIT_FAILURE;
            }
        }
        else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            Usage(stdout);
            return EXIT_SUCCESS;
        }
        else {
            if (contentPathArg != nullptr) {
                fprintf(stderr, "Error: content path already specified\n");
                return EXIT_FAILURE;
            }

            contentPathArg = argv[i];
        }
    }

    if (corePathArg == nullptr || contentPathArg == nullptr) {
        Usage(stderr);
        return EXIT_FAILURE;
    }

    Logger logger;

    if (!logger.init()) {
        fprintf(stderr, "Error: could not initialize the logger");
        return EXIT_FAILURE;
    }

    logger.setLevel(level);

    Config config;

    if (!config.init(configPathArg, contentPathArg, corePathArg, &logger)) {
        fprintf(stderr, "Error: could not initialize the configuration component");
        logger.destroy();
        return EXIT_FAILURE;
    }

    Video video;

    if (!video.init(&logger)) {
        fprintf(stderr, "Error: could not initialize the video component");
        config.destroy();
        logger.destroy();
        return EXIT_FAILURE;
    }

    video.destroy();
    config.destroy();
    logger.destroy();

    return EXIT_SUCCESS;
}
