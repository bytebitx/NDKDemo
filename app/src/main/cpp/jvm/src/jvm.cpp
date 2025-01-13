//
// Created by geely on 2024/5/8.
//

#include <jni.h>

JavaVM *jvm = nullptr;

void setJvm(JavaVM *vm) {
    jvm = vm;
}

JavaVM *getJvm() {
    return jvm;
}
