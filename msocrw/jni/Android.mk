# A simple test for the minimal standard C++ library
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := msocrw
LOCAL_SRC_FILES := testocr.cpp
LOCAL_CFLAGS := -fPIC -I${NDK_PROJECT_PATH}/../opencv/sdk/native/jni/include -I${NDK_PROJECT_PATH}/../zxing/include
LOCAL_LDFLAGS := -L${NDK_PROJECT_PATH}/../mslib/${TARGET_ARCH} -lmsocr -L${NDK_PROJECT_PATH}/../opencv/sdk/native/libs/${TARGET_ARCH} -lopencv_java3 -L${NDK_PROJECT_PATH}/../zxing/lib/${TARGET_ARCH} -lzxing_core
include $(BUILD_SHARED_LIBRARY)
