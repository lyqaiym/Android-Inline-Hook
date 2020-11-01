#ifndef _LOG_H
#define _LOG_H
#include <android/log.h>

#define LOG_TAG "inlinehook" // 这个是自定义的LOG的标识
#define LOG_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,  __VA_ARGS__) // 定义LOGD类型
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, fmt, ##args);
#endif