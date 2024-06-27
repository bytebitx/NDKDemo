#include <jni.h>
#include <string>
#include <CPPLog.h>
//#include <people.h> // cmake中增加了include_directories(people/)，因此可以使用箭头引入

extern "C" JNIEXPORT jstring

JNICALL
Java_com_bytebitx_ndkdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
//    people p;
LOGD("test");
    std::string hello = "Hello from C++";
    return env->NewStringUTF( "Hello from C++");
}