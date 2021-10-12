// It seems SDL doesn't like main being in a C++ file.
int Main(int argc, char const* argv[]);

int main(int argc, char const* argv[]) {
    return Main(argc, argv);
}
