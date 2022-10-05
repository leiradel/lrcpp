#include <SDL.h>

// It seems SDL doesn't like main being in a C++ file.
int Main(int argc, char* argv[]);

int main(int argc, char* argv[]) {
    return Main(argc, argv);
}
