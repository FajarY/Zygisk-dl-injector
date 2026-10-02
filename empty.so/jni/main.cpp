#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, "libempty.so", __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "libempty.so", __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, "libempty.so", __VA_ARGS__)

extern "C" __attribute__((visibility("default")))
volatile int should_continue = 0;

extern "C" __attribute__((constructor))
void on_load()
{
    LOGI("loaded!");
}

extern "C" __attribute__((visibility("default")))
int waiter_should_continue(const void* appSpecializeArgs, void* api, void* env)
{
    LOGI("waiter got %p %p %p", appSpecializeArgs, api, env);
    return should_continue;
}