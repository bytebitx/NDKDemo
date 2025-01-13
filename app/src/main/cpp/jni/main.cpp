//
// Created by geely on 2024/4/30.
//
#include <jni.h>
#include <string>
#include <CPPLog.h>
#include <jvm.h>
#include "Add.h"

//由于 FFmpeg 库是 C 语言实现的，告诉编译器按照 C 的规则进行编译
// 必现这么引入，否则会导致找不到对应的方法，如：undefined symbol: avcodec_configuration()
// did you mean: extern "C" av_version_info
extern "C" {
#include "libavutil/avutil.h"
#include "libavcodec/avcodec.h"
}


#define JAVA_CLASS "com/bytebitx/ndkdemo/MainActivity"

JNIEXPORT JNICALL jobject accessBasicVar(JNIEnv *env, jobject thiz, jlong l, jboolean b, jstring s) {
    LOGD("dynamic register accessBasicVar");
    // 根据对象获取对应的class
    jclass cls = env->GetObjectClass(thiz);
    int sum = add(1, 2);
    LOGD("sum:%d", sum);

    jfieldID lField = env->GetFieldID(cls, "basicL", "J");
    jfieldID bField = env->GetFieldID(cls, "basicB", "Z");
    jfieldID sField = env->GetFieldID(cls, "basicS", "Ljava/lang/String;");
    jlong basicL = 2 + l;
    jboolean basicB = !b;
    jstring basicS = env->NewStringUTF("this is basic jni string");
    env->SetLongField(thiz, lField, basicL);
    env->SetBooleanField(thiz, bField, basicB);
    env->SetObjectField(thiz, sField, basicS);

    jfieldID sLField = env->GetStaticFieldID(cls, "sBasicL", "J");
    jfieldID sBField = env->GetStaticFieldID(cls, "sBasicB", "Z");
    jfieldID sSField = env->GetStaticFieldID(cls, "sBasicS", "Ljava/lang/String;");
    jlong sLong = 1 + l;
    jboolean sBoolean = b;
    jstring sString = env->NewStringUTF("this is new jni string");
    env->SetStaticLongField(cls, sLField, sLong);
    env->SetStaticBooleanField(cls, sBField, sBoolean);
    env->SetStaticObjectField(cls, sSField, sString);

    jclass listCls = env->FindClass("java/util/ArrayList");
    jmethodID cmid = env->GetMethodID(listCls, "<init>", "()V");
    jobject arrayList = env->NewObject(listCls, cmid);
    jmethodID addMid = env->GetMethodID(listCls, "add", "(Ljava/lang/Object;)Z");

    jclass longCls = env->FindClass("java/lang/Long");
    jmethodID longConstructorMid = env->GetMethodID(longCls, "<init>", "(J)V");
    jobject longObj = env->NewObject(longCls, longConstructorMid, basicL);
    jobject sLongObj = env->NewObject(longCls, longConstructorMid, sLong);
    env->CallBooleanMethod(arrayList, addMid, longObj);
    env->CallBooleanMethod(arrayList, addMid, sLongObj);

    jclass booleanCls = env->FindClass("java/lang/Boolean");
    jmethodID boolConstructorMid = env->GetMethodID(booleanCls, "<init>", "(Z)V");
    jobject boolObj = env->NewObject(booleanCls, boolConstructorMid, basicB);
    jobject sBoolObj = env->NewObject(booleanCls, boolConstructorMid, sBoolean);
    env->CallBooleanMethod(arrayList, addMid, boolObj);
    env->CallBooleanMethod(arrayList, addMid, sBoolObj);

    env->CallBooleanMethod(arrayList, addMid, basicS);
    env->CallBooleanMethod(arrayList, addMid, sString);

    return arrayList;
}


JNIEXPORT void JNICALL accessReferenceVar(JNIEnv *env, jobject thiz,
                                                          jobject test_call_back) {

    jclass testCls = env->GetObjectClass(test_call_back);
    jmethodID frameMid = env->GetMethodID(testCls, "onVideoFrame", "(IJ)V");
    jlong bitrate = 1;
    env->CallVoidMethod(test_call_back, frameMid, 1, bitrate);

    jmethodID successMid = env->GetMethodID(testCls, "onConnectSuccess", "(II)V");
    env->CallVoidMethod(test_call_back, successMid, 0, 0);

    jmethodID errMid = env->GetMethodID(testCls, "onConnectError", "()V");
    env->CallVoidMethod(test_call_back, errMid);

}


//为什么 thiz 和 instance 不是同一个内存地址？
//JNI 是基于句柄的：jobject 是一个指向 JVM 内部数据结构的句柄，而不是直接的内存指针。不同的 jobject 可以引用相同的 Java 对象。
//不同的调用上下文：在 JNI 方法中，thiz 代表的是调用该方法的对象实例，而 instance 是作为参数传递的对象引用。
// 尽管它们引用相同的 Java 对象，但它们在 JNI 中被视为不同的句柄。
JNIEXPORT JNICALL void accessMainInstance(JNIEnv *env, jobject thiz, jobject main_activity) {
    LOGD("dynamic register nativeMainActivity");
    if (env->IsSameObject(thiz, main_activity)) { // true
        LOGD("same object");
    } else {
        LOGD("not same object");
    }
    LOGD("Java object address: %p\n", thiz);
    LOGD("Java object address: %p\n", main_activity);
    jclass cls = env->GetObjectClass(thiz); // 使用thiz和main_activity是相同效果
    jmethodID resumeMid = env->GetMethodID(cls, "onResume", "()V");
    env->CallVoidMethod(thiz, resumeMid);
}


JNIEXPORT JNICALL jlong staticAccessL(JNIEnv *env, jobject thiz, jlong l) {
    LOGD("dynamic register accessStaticL");
    // 根据对象获取对应的class
    jclass cls = env->GetObjectClass(thiz);
    jmethodID setLMid = env->GetMethodID(cls, "setSBasicL", "(J)V");
    jlong sLong = 11 + l;
    env->CallVoidMethod(thiz, setLMid, sLong);
    return sLong;
}

jobject testObj = nullptr;
jmethodID fMid = nullptr;


void *testCallBack(void *) {
    JNIEnv *env;
    jint result = getJvm()->AttachCurrentThread(&env, nullptr);
    if (result == 0) { // 获取env实例成功
        jlong bitrate = 50;
        env->CallVoidMethod(testObj, fMid, 50, bitrate);
        env->DeleteGlobalRef(testObj);
    }
    getJvm()->DetachCurrentThread();
    pthread_exit(0);
}


JNIEXPORT void JNICALL accessCallBackByChildThread(JNIEnv *env, jobject thiz, jobject test_call_back) {
    jclass testCls = env->GetObjectClass(test_call_back);
    testObj = env->NewGlobalRef(test_call_back);
    fMid = env->GetMethodID(testCls, "onVideoFrame", "(IJ)V");
    pthread_t pthreadHandle;
    pthread_create(&pthreadHandle, nullptr, testCallBack, nullptr);
}



// 固定写法
extern "C" jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    setJvm(vm);
    JNIEnv *env;
    if (vm->GetEnv((void **)(&env), JNI_VERSION_1_6) != JNI_OK) {
        LOGD("JNI version not supported.");
        return JNI_FALSE;
    }
    jclass cls = env->FindClass(JAVA_CLASS);
    if (cls == nullptr) {
        return JNI_FALSE;
    }

    // 通过这种方式创建MainActivity对象，然后设置对应变量的值，在MainActivity更新UI的时候获取到变量不是这个方法
    // 改变之后的变量。因为通过这种方式创建的MainActivity变量和UI上显示的MainActivity变量不是同一个变量。
//    jfieldID normalLField = env->GetFieldID(cls, "normalL", "J");
//    jmethodID constructorMid = env->GetMethodID(cls, "<init>", "()V");
//    jobject mainObj = env->NewObject(cls, constructorMid);
//    jlong newNormalL = 11;
//    env->SetLongField(mainObj, normalLField, newNormalL);


    //-------------------------------------注册方法---------------------------------------


    JNINativeMethod jniNativeMethod[] = {
            {"accessBasicVar", "(JZLjava/lang/String;)Ljava/util/ArrayList;", (void*) accessBasicVar},
            {"accessReferenceVar", "(Lcom/bytebitx/ndkdemo/MainActivity$TestCallBack;)V", (void*) accessReferenceVar},
            {"accessMainInstance", "(Lcom/bytebitx/ndkdemo/MainActivity;)V", (void*) accessMainInstance},
            {"accessCallBackByChildThread", "(Lcom/bytebitx/ndkdemo/MainActivity$TestCallBack;)V", (void*) accessCallBackByChildThread},

    };

    if (env->RegisterNatives(cls, jniNativeMethod, 4) < JNI_OK) {
        return JNI_FALSE;
    }


    //-------------------------------------注册kotlin的静态方法---------------------------------------

    jclass clsCompanion = env->FindClass("com/bytebitx/ndkdemo/MainActivity$Companion");
    if (clsCompanion == nullptr) {
        return JNI_FALSE;
    }
    JNINativeMethod jniNativeCompanionMethod[] = {
            {"staticAccessNormalL", "(J)J", (void*) staticAccessL},
    };
    if (env->RegisterNatives(clsCompanion, jniNativeCompanionMethod, 1) < JNI_OK) {
        return JNI_FALSE;
    }

    //-------------------------------------实例化java类并调用java类的方法---------------------------------------
    jclass realBackCls = env->FindClass("com/bytebitx/ndkdemo/MainActivity$RealPlayCallBack");
    jmethodID cmid = env->GetMethodID(realBackCls, "<init>", "()V");
    jobject realObj = env->NewObject(realBackCls, cmid);
    jmethodID framesMid = env->GetMethodID(realBackCls, "onVideoFrame", "(IJ)V");
    jlong bitrate = 1;
    env->CallVoidMethod(realObj, framesMid, 0, bitrate);

    jmethodID connectSucMid = env->GetMethodID(realBackCls, "onConnectSuccess", "(II)V");
    env->CallVoidMethod(realObj, connectSucMid, 1, 1);

    jmethodID connectErrMid = env->GetMethodID(realBackCls, "onConnectError", "()V");
    env->CallVoidMethod(realObj, connectErrMid);

    LOGD("JNI on load success");
    return JNI_VERSION_1_6;
}










extern "C"
JNIEXPORT jstring JNICALL
Java_com_bytebitx_ndkdemo_MainActivity_accessLocalReference(JNIEnv *env, jobject thiz) {
    jclass stringCls = env->FindClass("java/lang/String");
    jmethodID mid = env->GetMethodID(stringCls, "<init>", "(Ljava/lang/String;)V");
    jstring str = env->NewStringUTF("this is new string");
    jobject strObj = env->NewObject(stringCls, mid, str);
    // NewLocalRef方法使用比较少，因为默认就是局部引用
    jstring result = static_cast<jstring>(env->NewLocalRef(strObj));
    env->DeleteLocalRef(strObj);
    return result;
//    return static_cast<jstring>(env->NewLocalRef(strObj));
//    return static_cast<jstring>(strObj);
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_bytebitx_ndkdemo_MainActivity_accessGlobalReference(JNIEnv *env, jobject thiz) {
    jclass stringCls = env->FindClass("java/lang/String");
    jmethodID mid = env->GetMethodID(stringCls, "<init>", "(Ljava/lang/String;)V");
    jstring str = env->NewStringUTF("this is new string");
    jobject strObj = env->NewObject(stringCls, mid, str);
    static jstring result = nullptr;
    if (result == nullptr) {
        result = static_cast<jstring>(env->NewGlobalRef(strObj));
    } else {
        const char *cstr = env->GetStringUTFChars(result, JNI_FALSE);
        LOGD("result cached : %s", cstr);
    }
//    env->DeleteGlobalRef(result);
    return result;

}
extern "C"
JNIEXPORT void JNICALL
Java_com_bytebitx_ndkdemo_MainActivity_accessWeakReference(JNIEnv *env, jobject thiz) {
    jclass stringCls = env->FindClass("java/lang/String");
    jmethodID mid = env->GetMethodID(stringCls, "<init>", "(Ljava/lang/String;)V");
    jstring str = env->NewStringUTF("this is new string");
    jobject strObj = env->NewObject(stringCls, mid, str);
    static jstring result = nullptr;
    if (result == nullptr) {
        result = static_cast<jstring>(env->NewWeakGlobalRef(strObj));
    } else {
        const char *cstr = env->GetStringUTFChars(result, JNI_FALSE);
        LOGD("result cached : %s", cstr);
    }
    jboolean isGc = env->IsSameObject(result, nullptr);
    if (isGc) {

    }
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_bytebitx_ndkdemo_MainActivity_getFFmpegVersion(JNIEnv *env, jobject thiz) {
    char version[1024 * 4] = {0};
    strcat(version, "ffmpeg : ");
    strcat(version, av_version_info());
    strcat(version, "\nlibavcodec : ");
    strcat(version, AV_STRINGIFY(LIBAVCODEC_VERSION));
    strcat(version, "\nlibavformat : ");
    strcat(version, AV_STRINGIFY(LIBAVFORMAT_VERSION));
    strcat(version, "\navcodec_configure : \n");
    strcat(version, avcodec_configuration());
//    ffmpegmain();
    return env->NewStringUTF(version);
}