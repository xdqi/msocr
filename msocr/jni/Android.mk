# A simple test for the minimal standard C++ library
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := msocr
LOCAL_SRC_FILES := testocr.cpp
LOCAL_CFLAGS := -fPIE
LOCAL_LDFLAGS := -L../../mslib/${TARGET_ARCH} -lmsocr -pie
include $(BUILD_EXECUTABLE)
