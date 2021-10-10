#include "Player.h"

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <string>
#include <vector>

static void Usage(FILE* out) {
    fprintf(out, "Usage: sdl2lrcpp [options...] <content path>\n\n");
    fprintf(out, "-L, --libretro   The path to the libretro core\n");
    fprintf(out, "-c, --config     The path to the configuration file (optional)\n");
    fprintf(out, "--appendconfig   Paths for additional configuration files, separated by commas\n");
    fprintf(out, "-v, --verbose    Increase the verboseness one level (up to two levels)\n");
    fprintf(out, "-h, --help       Shows this help screen and exit\n");
    fprintf(out, "<content path>   The path to the content file to use with the core\n");
}

extern "C" int Main(int argc, char const* argv[]) {
    char const* corePath = nullptr;
    std::vector<std::string> configPaths;
    bool configSpecified = false;
    char const* contentPath = nullptr;
    retro_log_level level = RETRO_LOG_WARN;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--libretro") == 0) {
            if (++i == argc) {
                fprintf(stderr, "Error: missing core path after --libretro\n");
                return EXIT_FAILURE;
            }

            if (corePath != nullptr) {
                fprintf(stderr, "Error: core path already specified\n");
                return EXIT_FAILURE;
            }

            corePath = argv[i];
        }
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config") == 0) {
            if (++i == argc) {
                fprintf(stderr, "Error: missing configuration path after --config\n");
                return EXIT_FAILURE;
            }

            if (configSpecified) {
                fprintf(stderr, "Error: configuration path already specified\n");
                return EXIT_FAILURE;
            }

            configPaths.emplace_back(argv[i]);
            configSpecified = true;
        }
        else if (strcmp(argv[i], "--appendconfig") == 0) {
            if (++i == argc) {
                fprintf(stderr, "Error: missing configuration path after --config\n");
                return EXIT_FAILURE;
            }

            char const* aux = argv[i];

            while (*aux != 0) {
                char const* const comma = strchr(aux, ',');

                if (comma == nullptr) {
                    configPaths.emplace_back(aux);
                    break;
                }

                configPaths.emplace_back(aux, comma - aux);
                aux = comma + 1;
            }
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
            if (contentPath != nullptr) {
                fprintf(stderr, "Error: content path already specified\n");
                return EXIT_FAILURE;
            }

            contentPath = argv[i];
        }
    }

    if (corePath == nullptr || contentPath == nullptr) {
        Usage(stderr);
        return EXIT_FAILURE;
    }

    Player player;

    if (!player.init(configPaths, corePath, contentPath, level)) {
        return EXIT_FAILURE;
    }

    player.run();
    player.destroy();

    return EXIT_SUCCESS;
}
