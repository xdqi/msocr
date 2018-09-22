# A simple test for the minimal standard C++ library
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := msocrw
LOCAL_SRC_FILES := testocr.cpp
LOCAL_CFLAGS := -fPIC
LOCAL_LDFLAGS := -L${NDK_PROJECT_PATH}/../mslib/${TARGET_ARCH} -lmsocr
include $(BUILD_SHARED_LIBRARY)
