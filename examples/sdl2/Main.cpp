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
    fprintf(out, "-v, --verbose    Increase the verboseness\n");
    fprintf(out, "-h, --help       Shows this help screen and exit\n");
    fprintf(out, "<content path>   The path to the content file to use with the core\n");
}

static void CopyFiles() {
    static char const* const files[] = {
        "mdkPSX_10.bin", "mdkPSX_11.bin", "mdkPSX_12.bin", "mdkPSX_13.bin", "mdkPSX_14.bin", "mdkPSX_15.bin",
        "mdkPSX_16.bin", "mdkPSX_17.bin", "mdkPSX_18.bin", "mdkPSX_19.bin", "mdkPSX_1.bin", "mdkPSX_20.bin",
        "mdkPSX_21.bin", "mdkPSX_22.bin", "mdkPSX_23.bin", "mdkPSX_24.bin", "mdkPSX_25.bin", "mdkPSX_26.bin",
        "mdkPSX_27.bin", "mdkPSX_28.bin", "mdkPSX_29.bin", "mdkPSX_2.bin", "mdkPSX_30.bin", "mdkPSX_3.bin",
        "mdkPSX_4.bin", "mdkPSX_5.bin", "mdkPSX_6.bin", "mdkPSX_7.bin", "mdkPSX_8.bin", "mdkPSX_9.bin",
        "mdkPSX.cue", "swanstation_libretro_android.so", "scph5501.bin"
    };

    auto const prefPath = std::string(SDL_GetPrefPath("antstream", "pacman"));
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Pref path: \"%s\"", prefPath.c_str());

    for (size_t i = 0; i < sizeof(files) / sizeof(files[0]); i++) {
        auto const outPath = prefPath + files[i];
        SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Extracting to \"%s\"", outPath.c_str());

        SDL_RWops* const in = SDL_RWFromFile(files[i], "rb");
        FILE* const out = fopen(outPath.c_str(), "wb");

        for (;;) {
            uint8_t buffer[4096];
            size_t const num_read = SDL_RWread(in, buffer, 1, sizeof(buffer));

            if (num_read == 0) {
                break;
            }

            fwrite(buffer, 1, num_read, out);
        }

        fclose(out);
        SDL_FreeRW(in);
    }
}

extern "C" int Main(int argc, char* argv[]) {
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

    char const* corePath = nullptr;
    std::vector<std::string> configPaths;
    bool configSpecified = false;
    char const* contentPath = nullptr;
    int verboseness = 0;

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

            configPaths.emplace(configPaths.begin(), argv[i]);
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
            verboseness++;
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

    CopyFiles();

    auto const prefPath = std::string(SDL_GetPrefPath("antstream", "mdk"));

    std::string const& corePathStr = prefPath + "swanstation_libretro_android.so";
    corePath = corePathStr.c_str();

    std::string const& contentPathStr = prefPath + "mdkPSX.cue";
    contentPath = contentPathStr.c_str();

    if (corePath == nullptr || contentPath == nullptr) {
        Usage(stderr);
        return EXIT_FAILURE;
    }

    Player player;

    if (!player.init(configPaths, corePath, contentPath, verboseness)) {
        return EXIT_FAILURE;
    }

    player.run();
    player.destroy();

    return EXIT_SUCCESS;
}
