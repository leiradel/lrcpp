#include "Config.h"
#include "Logger.h"
#include "Video.h"

#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <string>

static Logger s_logger;
static Config s_config;
static Video s_video;

#ifdef WIN32
    #define _MAX_PATH PATH_MAX

    extern "C" {
        #include "realpath.c"
    }
#endif

static void Usage(FILE* out) {
    fprintf(out, "Usage:");
}

static bool RealPath(char const* path, std::string* realPath) {
    char temp[PATH_MAX];

    if (realpath(path, temp) == NULL) {
        fprintf(stderr, "Error: couldn't get the absolute path for \"%s\": %s", path.c_str(), strerror(errno));
        return false;
    }

    *realPath = temp;
    return true;
}

int main(int argc, char const* argv[]) {
    char const* corePathArg = nullptr;
    char const* configPathArg = nullptr;
    char const* contentPathArg = nullptr;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-L") == 0 || strcmp(argv[i], "--libretro")) {
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
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--config")) {
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

    std::string corePath;
    std::string contentPath;

    if (!RealPath(corePathArg, &corePath) || !RealPath(contentPathArg, &contentPath)) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
