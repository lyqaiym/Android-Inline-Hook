#ifndef HOOK_INLINE_H_INCLUDED
#define HOOK_INLINE_H_INCLUDED
#include <stdbool.h>
struct hook_reg {
    long ARM_r0; long ARM_r1; long ARM_r2; long ARM_r3;
    long ARM_r4; long ARM_r5; long ARM_r6; long ARM_r7;
    long ARM_r8; long ARM_r9; long ARM_r10;long ARM_r11;
    long ARM_r12;long ARM_sp; long ARM_lr; long ARM_cpsr;
};
typedef void (*hook_func)(struct hook_reg *reg);
bool hook_inline_make(const char *library, long address, hook_func func, bool isArm);
#endif // HOOK_INLINE_H_INCLUDED