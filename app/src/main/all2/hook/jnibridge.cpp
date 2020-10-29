#include <jni.h>
#include <bits/signal_types.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
#include "log.h"

extern "C" {
#include "hooktest.h"
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hook_NativeTry_testHook(JNIEnv *env, jclass clazz, jobject obj) {
    LOG_DEBUG("testHook");
//    test();
    ModifyIBored();
//    target_foo();
}