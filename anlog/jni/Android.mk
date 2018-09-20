# A simple test for the minimal standard C++ library
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := anlog 
LOCAL_SRC_FILES := anlog.c
include $(BUILD_SHARED_LIBRARY)
