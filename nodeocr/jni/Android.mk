# A simple test for the minimal standard C++ library
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := nodeocr
LOCAL_SRC_FILES := nodeocr.cc
LOCAL_CFLAGS := -fPIC -I${NDK_PROJECT_PATH}/../node/${TARGET_ARCH}/include/node

# does not work as default ldflags contains -Wl,--no-undefined
LOCAL_LDFLAGS := -L${NDK_PROJECT_PATH}/../msocrw/libs/${TARGET_ARCH} -lmsocrw -Wl,--as-needed
include $(BUILD_SHARED_LIBRARY)
