LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -fPIC -std=c99
LOCAL_LDFLAGS += -fPIC 
LOCAL_LDLIBS := -llog

LOCAL_MODULE    := auto_start_recording 
LOCAL_SRC_FILES := auto_start_recording.c

include $(BUILD_SHARED_LIBRARY)
