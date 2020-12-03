#ifndef LRCPP_DYNLIB_H__
#define LRCPP_DYNLIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
    #endif

    #include <windows.h>

    typedef HMODULE dynlib_t;

    #define dynlib_open(path) LoadLibrary(path)
    #define dynlib_symbol(lib, name) (void*)GetProcAddress(lib, name)
    #define dynlib_close(lib) FreeLibrary(lib)

    char const* dynlib_error(void);
#else
    #include <dlfcn.h>

    typedef void* dynlib_t;

    #define dynlib_open(path) dlopen(path, RTLD_LAZY)
    #define dynlib_symbol(lib, name) dlsym(lib, name)
    #define dynlib_close(lib) dlclose(lib)
    #define dynlib_error() dlerror()
#endif

#ifdef __cplusplus
}
#endif

#endif /* LRCPP_DYNLIB_H__ */
