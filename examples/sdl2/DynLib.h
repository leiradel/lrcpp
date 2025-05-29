#pragma once

#include <string>

#ifdef _WIN32
#   include <stdio.h>
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
    typedef HMODULE dynlib_t;
#else
#   include <dlfcn.h>
    typedef void* dynlib_t;
#endif

// C++ wrapper around lrcpp/dynlib.h
class DynLib {
public:
    DynLib() : handle(nullptr) {}
    ~DynLib() { unload(); }

    bool load(const std::string& path) {
        unload(); // Unload previous if any
#ifdef _WIN32
        handle = LoadLibrary(path.c_str());
#else
        handle = dlopen(path.c_str(), RTLD_LAZY);
#endif
        return handle != nullptr;
    }

    void unload() {
        if (handle) {
#ifdef _WIN32
            FreeLibrary(lib);
#else
            dlclose(handle);
#endif
            handle = nullptr;
        }
    }

    void* getSymbol(const char* name) const {
        if (!handle) return nullptr;
#ifdef _WIN32
        return static_cast<void *>(GetProcAddress(handle, name));
#else
        return dlsym(handle, name);
#endif
    }

    const char* error() const {
#ifdef _WIN32
        static char msg[ 512 ];
        
        DWORD err = GetLastError();

        DWORD res = FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            err,
            MAKELANGID( LANG_ENGLISH, SUBLANG_DEFAULT ),
            msg,
            sizeof( msg ) - 1,
            NULL
        );

        if ( res == 0 )
        {
            snprintf( msg, sizeof( msg ) - 1, "Error %lu", err );
            msg[ sizeof( msg ) - 1 ] = 0;
        }

        return msg;
#else
        return dlerror();
#endif
    }

    bool isLoaded() const { return handle != nullptr; }

private:
    dynlib_t handle;
};