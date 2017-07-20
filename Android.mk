LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := Acore
LOCAL_SRC_FILES := Acore.cpp

include $(BUILD_SHARED_LIBRARY)
