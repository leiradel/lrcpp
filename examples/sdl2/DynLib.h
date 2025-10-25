#pragma once

#include <string>

#ifdef _WIN32
#   include <stdio.h>
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#else
#   include <dlfcn.h>
#endif

// C++ wrapper around lrcpp/dynlib.h
class DynLib {
public:
    DynLib() : _handle(nullptr) {}
    ~DynLib() { unload(); }

    bool load(const std::string& path) {
        unload(); // Unload previous if any
#ifdef _WIN32
        _handle = LoadLibrary(path.c_str());
#else
        _handle = dlopen(path.c_str(), RTLD_LAZY);
#endif
        return _handle != nullptr;
    }

    void unload() {
        if (_handle) {
#ifdef _WIN32
            FreeLibrary(_handle);
#else
            dlclose(_handle);
#endif
            _handle = nullptr;
        }
    }

    void* getSymbol(const char* name) const {
        if (!_handle) return nullptr;
#ifdef _WIN32
        return (void *)GetProcAddress(_handle, name);
#else
        return dlsym(_handle, name);
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

    bool isLoaded() const { return _handle != nullptr; }

private:
#ifdef _WIN32
    HMODULE _handle;
#else
    void* _handle;
#endif
};
