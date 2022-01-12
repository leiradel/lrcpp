LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/$(SDL_PATH)/include \
	$(LOCAL_PATH)/../../../.. \
	$(LOCAL_PATH)/../../../../../../include

# Add your application source files here...
LOCAL_SRC_FILES := \
	../../../../Audio.cpp \
	../../../../Config.cpp \
	../../../../dynlib.c \
	../../../../Entry.c \
	../../../../Input.cpp \
	../../../../Logger.cpp \
	../../../../Main.cpp \
	../../../../Perf.cpp \
	../../../../Player.cpp \
	../../../../Video.cpp \
	../../../../../../src/Components.cpp \
	../../../../../../src/Core.cpp \
	../../../../../../src/CoreFsm.cpp \
	../../../../../../src/Frontend.cpp

LOCAL_SHARED_LIBRARIES := SDL2

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -llog

include $(BUILD_SHARED_LIBRARY)
