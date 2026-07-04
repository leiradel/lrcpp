#include "Player.h"

#include <stdlib.h>
#include <string>
#include <vector>

extern "C" int Main(int argc, char const* argv[]) {
    std::vector<std::string> configPaths;

    for (int i = 1; i < argc; i++) {
        configPaths.emplace_back(argv[i]);
    }

    Player player;

    if (!player.init(configPaths)) {
        return EXIT_FAILURE;
    }

    player.run();
    player.destroy();

    return EXIT_SUCCESS;
}
