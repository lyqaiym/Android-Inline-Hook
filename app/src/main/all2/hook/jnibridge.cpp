#include <jni.h>
#include <bits/signal_types.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "log.h"

extern "C" {
#include "hooktest.h"
}

typedef void (*target_foo)(void);

extern "C"
JNIEXPORT void JNICALL
Java_com_example_hook_NativeTry_testHook(JNIEnv *env, jclass clazz, jobject obj) {
    LOG_DEBUG("testHook");
//    test();
    void *handler = dlopen("libtarget.so", RTLD_NOW);
    target_foo foo = (target_foo) dlsym(handler, "target_foo");
    ModifyIBored();
    LOG_DEBUG("foo1");
    foo();
    LOG_DEBUG("foo2");
    foo();
}