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
//    test();
    void *handler = dlopen("libtarget.so", RTLD_NOW);
    target_foo foo = (target_foo) dlsym(handler, "target_foo");
    int target_offset;
#if defined(__arm__)
    LOG_DEBUG("testHook_32");
    //    E:\develop\sdk\ndk\21.1.6352462\toolchains\arm-linux-androideabi-4.9\prebuilt\windows-x86_64\bin\arm-linux-androideabi-objdump.exe -d
//    app\build\intermediates\cmake\debug\obj\armeabi-v7a\libtarget.so
//    > app\src\main\all2\hook\libtarget_v7.txt
target_offset = 0x7c0;
#elif defined(__aarch64__)
    LOG_DEBUG("testHook_64");
//    E:\develop\sdk\ndk\21.1.6352462\toolchains\aarch64-linux-android-4.9\prebuilt\windows-x86_64\bin\aarch64-linux-android-objdump.exe -d
//    app\build\intermediates\cmake\debug\obj\arm64-v8a\libtarget.so
//    > app\src\main\all2\hook\libtarget_v64.txt
    target_offset = 0x690;
#endif
    ModifyIBored("libtarget.so", target_offset);
    LOG_DEBUG("foo1");
    foo();
    UnInlineHook("libtarget.so",target_offset);
    LOG_DEBUG("foo2");
    foo();
}