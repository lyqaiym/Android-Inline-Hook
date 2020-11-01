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
//    E:\develop\sdk\ndk\21.1.6352462\toolchains\arm-linux-androideabi-4.9\prebuilt\windows-x86_64\bin\arm-linux-androideabi-objdump.exe -d
//    app\build\intermediates\cmake\debug\obj\armeabi-v7a\libtarget.so
//    > app\src\main\all2\hook\li btarget_v7.txt
//    ModifyIBored("libtarget.so",0x7c0);
//    E:\develop\sdk\ndk\21.1.6352462\toolchains\aarch64-linux-android-4.9\prebuilt\windows-x86_64\bin\aarch64-linux-android-objdump.exe -d
//    app\build\intermediates\cmake\debug\obj\arm64-v8a\libtarget.so
//    > app\src\main\all2\hook\libt arget_v64.txt
    ModifyIBored("libtarget.so",0x690);
    LOG_DEBUG("foo1");
    foo();
    LOG_DEBUG("foo2");
    foo();
}