#ifndef _HOOKTEST_H
#define _HOOKTEST_H

#if defined(__arm__)
typedef void (*callback)(pt_regs *regs);
#elif defined(__aarch64__)
typedef void (*callback)(user_pt_regs *regs);
#endif
void ModifyIBored(char* __filename,int target_offset,callback callback1);
bool UnInlineHook(char *__filename, int target_offset);
#endif