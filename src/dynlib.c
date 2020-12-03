#include "dynlib.h"

#ifdef _WIN32

#include <stdio.h>

char const* dynlib_error(void) {
    static char msg[512];

    DWORD err = GetLastError();

    DWORD res = FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        err,
        MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
        msg,
        sizeof(msg) - 1,
        NULL
    );

    if (res == 0) {
        snprintf(msg, sizeof(msg), "Error %lu", err);
    }

    return msg;
}

#endif
