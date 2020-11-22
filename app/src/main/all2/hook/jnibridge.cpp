#include <jni.h>
#include <bits/signal_types.h>
#include <signal.h>
#include <setjmp.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <asm/ptrace.h>
#include "log.h"

extern "C" {
#include "hooktest.h"
}

typedef void (*target_foo)(void);

/**
 * 用户自定义的stub函数，嵌入在hook点中，可直接操作寄存器等改变游戏逻辑操作
 * 这里将R0寄存器锁定为0x333，一个远大于30的值
 * @param regs 寄存器结构，保存寄存器当前hook点的寄存器信息
 */
#if defined(__arm__)
void targetCall(pt_regs *regs) //参数regs就是指向栈上的一个数据结构，由第二部分的mov r0, sp所传递。
{
    long r1 =regs->uregs[1];
    long r2 =regs->uregs[2];
    LOGI("targetCall.r0=%ld,r1=%ld",r1, r2);
    regs->uregs[1] = 0x3;
}

#elif defined(__aarch64__)
void targetCall(user_pt_regs *regs) //参数regs就是指向栈上的一个数据结构，由第二部分的mov r0, sp所传递。
{
    LOGI("targetCall.r9=%ld", regs->regs[9]);
    regs->regs[9] = 0x3;
}
#endif

int hook1 = 0;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hook_NativeTry_testTargetHook(JNIEnv *env, jclass clazz, jobject obj) {
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
    LOG_DEBUG("foo1");
    foo();
    if (hook1 == 0) {
        hook1 = 1;
        ModifyIBored("libtarget.so", target_offset, targetCall);
    }
//    UnInlineHook("libtarget.so",target_offset);
    LOG_DEBUG("foo2");
    foo();
}

#if defined(__arm__)

void libcCall(pt_regs *regs) //参数regs就是指向栈上的一个数据结构，由第二部分的mov r0, sp所传递。
{
    long r1 =regs->uregs[1];
    long r2 =regs->uregs[2];
    LOGI("libcCall.r1=%ld,r2=%ld",r1, r2);
    regs->uregs[2] = 0x2;
}

#elif defined(__aarch64__)

void libcCall(user_pt_regs *regs) //参数regs就是指向栈上的一个数据结构，由第二部分的mov r0, sp所传递。
{
    long r1 = regs->regs[0];
    long r2 = regs->regs[1];
    LOGI("libcCall.r1=%ld,r2=%ld", r1, r2);
    regs->regs[1] = 0x2;
}

#endif

//32 位 小米9手机的libc
//adb pull /apex/com.android.runtime/lib/bionic/libc.so
//00090b2e <div>:
//90b2e:	b570      	push	{r4, r5, r6, lr}
//90b30:	4604      	mov	r4, r0

//64 位 小米9手机的libc
//adb pull /apex/com.android.runtime/lib64/bionic/libc.so ./libc64.so
//00000000000bff88 <div>:
//bff88:	1ac10c08 	sdiv	w8, w0, w1
//bff8c:	7100001f 	cmp	w0, #0x0
//bff90:	1b018109 	msub	w9, w8, w1, w0
int hook2 = 0;
extern "C"
JNIEXPORT void JNICALL
Java_com_example_hook_NativeTry_testLibcHook(JNIEnv *env, jclass clazz) {
    dlopen("libc.so", RTLD_NOW);
    int target_offset;
#if defined(__arm__)
    LOG_DEBUG("testLibc_32");
    target_offset = 0x90b30;
#elif defined(__aarch64__)
    LOG_DEBUG("testLibc_64");
    target_offset = 0xbff90;
#endif
    div_t div1 = div(8, 3);  //求8除以3的商和余数
    if (hook2 == 0) {
        hook2 = 1;
        ModifyIBored("libc.so", target_offset, libcCall);
    }
    div_t div2 = div(8, 3);  //求8除以3的商和余数,变成除以2
    LOG_DEBUG("testLibc:q1=%d,r1=%d,q2=%d,r2=%d", div1.quot, div1.rem, div2.quot, div2.rem);
}