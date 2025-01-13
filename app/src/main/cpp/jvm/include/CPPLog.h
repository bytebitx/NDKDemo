

// ===============日志定义 开始==================//
/**
 *  ## 表示字符串连接符号，##__VA_ARGS__ 表示将__VA_ARGS__与前面的字符串连接成一个字符串
 *  #  表示为后面的字符串添加双引号 比如 #A 则代表 "A"
 *  下面的fmt，是格式化的输出字符串比如"%s %d"，...表示可变参数，后面的__VA_ARGS__与前面可变参数对应
 */



#include <jni.h>
#include <android/log.h>

#define LOG_TAG "JNI"

#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__))




// ===============日志定义 结束==================//

