LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE := botarena
LOCAL_SRC_FILES := main.cpp
LOCAL_CPPFLAGS += -std=c++11 -O3 -fexceptions -frtti -fno-math-errno
LOCAL_LDLIBS := -llog -landroid -lEGL -lGLESv1_CM -lm
LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
