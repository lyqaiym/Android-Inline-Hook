#ifndef _HOOKTEST_H
#define _HOOKTEST_H
int test();
void ModifyIBored(char* __filename,int target_offset);
bool UnInlineHook(char *__filename, int target_offset);
#endif