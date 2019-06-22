//
// Created by whui on 2019/6/22.
//

#ifndef CPPNATIVE_ANDROID_LOG_H
#define CPPNATIVE_ANDROID_LOG_H


//
// Created by whui on 2019/6/9.
//

#include <android/log.h>

#define LOG_TAG    "Native"
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


#endif //CPPNATIVE_ANDROID_LOG_H
