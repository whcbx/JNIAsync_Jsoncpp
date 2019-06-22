#include <jni.h>
#include <string>
//#include <android/log.h>
#include "android_log.h"
#include <pthread.h>
#include "com_hryt_iov_speech_cppnative_Sdk.h"
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <string>
//#include "jsoncpp/inc/json.h"
//#include "jsoncpp/inc/writer.h"
#include "json.h"
//extern "C" JNIEXPORT jstring JNICALL
//Java_com_hryt_iov_speech_cppnative_MainActivity_stringFromJNI(
//        JNIEnv *env,
//        jobject /* this */) {
//    std::string hello = "Hello from C++";
//    return env->NewStringUTF(hello.c_str());
//}
int mNeedDetach;
JavaVM *g_VM;
jobject g_obj;

void *download(void *p);

JNIEXPORT void JNICALL Java_com_hryt_iov_speech_cppnative_Sdk_nativeDownload(JNIEnv *env, jobject thiz) {

    //JavaVM是虚拟机在JNI中的表示，等下再其他线程回调java层需要用到
    env->GetJavaVM( &g_VM);
    // 生成一个全局引用保留下来，以便回调
    g_obj = env->NewGlobalRef( thiz);

    // 此处使用c语言开启一个线程，进行回调，这时候java层就不会阻塞，只是在等待回调
    pthread_t  tid;
    pthread_create(& tid,NULL, download,NULL);
    return ;
}

//在此处跑在子线程中，并回调到java层
void *download(void *p) {
    JNIEnv *env;

    //获取当前native线程是否有没有被附加到jvm环境中
    int getEnvStat = g_VM->GetEnv( (void **) &env,JNI_VERSION_1_6);
    if (getEnvStat == JNI_EDETACHED) {
        //如果没有， 主动附加到jvm环境中，获取到env
        if (g_VM->AttachCurrentThread(&env, NULL) != 0) {
            return NULL;
        }
        mNeedDetach = JNI_TRUE;
    }

    //通过全局变量g_obj 获取到要回调的类
    jclass javaClass = env->GetObjectClass( g_obj);

    if (javaClass == 0) {
        LOGI("Unable to find class");
        g_VM->DetachCurrentThread();
        return NULL;
    }

    //获取要回调的方法ID
    jmethodID javaCallbackId = env->GetMethodID(javaClass,
                                                "onProgressCallBack", "(Ljava/lang/String;I)I");
    if (javaCallbackId == NULL) {
        LOGI("Unable to find method:onProgressCallBack");
        return NULL;
    }
    LOGI("Enter sleep ...");
    sleep(5);
    LOGI("Exit Sleep ....");
    //执行回调
    std::string str = "hello callback from c++";
    Json::Value jsonValue;
    jsonValue["confirm"]["clicked"] = "Yes";
    jsonValue["FMCodeMin"]=87000;
    jsonValue["FMCodeMax"]=108000;
    jsonValue["AMCodeMin"]=531;
    jsonValue["AMCodeMax"]=1629;

//
//    "animals": {
//        "dog": [
//        {
//            "name": "Rufus",
//                    "age":15
//        },
//        {
//            "name": "Marty",
//                    "age": null
//        }
//        ]
//    }
//    std::string json_string
//    char * str1
//    std::string json_string ="{\"id":1,\"name\":\"jb51\",\"email\":\"admin@jb51.net\",\"interest\":[\"wordpress\",\"php\"]}" ;

    Json::Value jsonValue1;
    jsonValue1["id"] = 1;
    jsonValue1["name"]="Jack";
    jsonValue1["email"]="admin@jb51.net";
    jsonValue1["interest"].append("wordPress");
    jsonValue1["interest"].append("php");
    std::string json_string = jsonValue1.toStyledString();
    LOGI("json_string:  %s",json_string.c_str());

    Json::Value jsonValue2;
    Json::Value jsonValue3;

    jsonValue3["name"] = "Rufus";
    jsonValue3["age"] = 20;
    jsonValue2["animals"]["dog"].append(jsonValue3);
    jsonValue3["name"] = "Marty";
    jsonValue3["age"] = 30;
    jsonValue2["animals"]["dog"].append(jsonValue3);
    LOGI("jsonValue2:  %s",jsonValue2.toStyledString().c_str());

    Json::Reader jsonReader;
    Json::Value JsonRoot;
    jsonReader.parse(json_string,JsonRoot);
    int id = JsonRoot["id"].asInt();
    std::string name = JsonRoot["name"].asString();
    std::string email = JsonRoot["email"].asString();
    LOGI("id:  %d",id);
    LOGI("name:  %s",name.c_str());
    LOGI("email:  %s",email.c_str());


    Json::Value ObjectArray;
    ObjectArray = JsonRoot["interest"];
    std::string stringTmp;

    for (int k = 0; k < ObjectArray.size(); ++k)
    {
        //const Json::Value& jsonValueType = *it;
        stringTmp = ObjectArray[k].asString();
        LOGI("k --- stringTmp: %s ",stringTmp.c_str());
    }

    for (Json::ValueConstIterator it = ObjectArray.begin(); it != ObjectArray.end(); ++it)
    {
        //const Json::Value& jsonValueType = *it;
         stringTmp = (*it).asString();
         LOGI("stringTmp: %s ",stringTmp.c_str());
    }

    str = jsonValue.toStyledString();
//    env->CallIntMethod( g_obj, javaCallbackId,env->NewStringUTF(str.c_str()),25);
    env->CallIntMethod( g_obj, javaCallbackId,env->NewStringUTF(jsonValue2.toStyledString().c_str()),25);


    //释放当前线程
    if(mNeedDetach) {
        g_VM->DetachCurrentThread();
    }
    env = NULL;
    return NULL;
}


