#pragma once
#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "dlinjector", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "dlinjector", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "dlinjector", __VA_ARGS__)