
# Uncomment this if you're using STL in your project
# You can find more information here:
# https://developer.android.com/ndk/guides/cpp-support
# APP_STL := c++_shared

APP_ABI := armeabi-v7a arm64-v8a x86 x86_64

# Min runtime API level
APP_PLATFORM=android-16

# Enable C++11 and C99 in source code
APP_CPPFLAGS += -std=c++11
APP_CFLAGS += -std=c99
