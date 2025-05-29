#pragma once

#include "lrcpp/dynlib.h"
#include <string>

// C++ wrapper around lrcpp/dynlib.h
class DynLib {
public:
    DynLib() : handle(nullptr) {}
    ~DynLib() { unload(); }

    bool load(const std::string& path) {
        unload(); // Unload previous if any
        handle = dynlib_open(path.c_str());
        return handle != nullptr;
    }

    void unload() {
        if (handle) {
            dynlib_close(handle);
            handle = nullptr;
        }
    }

    void* getSymbol(const char* name) const {
        if (!handle) return nullptr;
        return dynlib_symbol(handle, name);
    }

    const char* error() const {
#ifdef _WIN32
        return dynlib_error(); // stub; Windows version may need implementation
#else
        return dynlib_error();
#endif
    }

    bool isLoaded() const { return handle != nullptr; }

private:
    dynlib_t handle;
};