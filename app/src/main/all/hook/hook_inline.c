#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <bits/sysconf.h>
#include <sys/mman.h>
#include "hook_inline.h"
#include "log.h"

long get_module_addr(pid_t pid, const char *module_name)
{
    char file_path[256];
    char file_line[512];
    if (pid < 0) {
        snprintf(file_path, sizeof(file_path), "/proc/self/maps");
    } else {
        snprintf(file_path, sizeof(file_path), "/proc/%d/maps", pid);
    }
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL) {
        return -1;
    }
    long addr_start = -1, addr_end = 0;
    while (fgets(file_line, sizeof(file_line), fp)) {
        if (strstr(file_line, module_name)) {
            if (2 == sscanf(file_line, "%8lx-%8lx", &addr_start, &addr_end)) {
                break;
            }
        }
    }
    fclose(fp);
    LOG_DEBUG("library :%s %lx-%lx, pid : %d\n", module_name, addr_start, addr_end, pid);
    return addr_start;
}

bool change_addr_writable(long address, bool writable) {
    long page_size = sysconf(_SC_PAGESIZE);
    //align address by page size
    long page_start = (address) & (~(page_size - 1));
    //change memory attribute
    if (writable == true) {
        return mprotect((void*)page_start, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) != -1;
    } else {
        return mprotect((void*)page_start, page_size, PROT_READ | PROT_EXEC) != -1;
    }
}

bool hook_inline_make(const char *library, long address, hook_func func,bool isArm) {
    //获取hook点在内存中的地址
    long base_addr = get_module_addr(-1, library);
    long hook_addr = base_addr + address;
    //获取shellcode中的符号地址
    extern long _shellcode_start_s;
    extern long _shellcode_end_s;
    extern long _hook_func_addr_s;
    extern long _stub_func_addr_s;
    void *p_shellcode_start = &_shellcode_start_s;
    void *p_shellcdoe_end = &_shellcode_end_s;
    void *p_hook_func = &_hook_func_addr_s;
    void *p_stub_func = &_stub_func_addr_s;
    //计算shellcode大小
    int shellcode_size = (int) (p_shellcdoe_end - p_shellcode_start);
    //新建shellcode
    void *shellcode = malloc(shellcode_size);
    memcpy(shellcode, p_shellcode_start, shellcode_size);
    //添加执行属性
    change_addr_writable((long) shellcode, true);
    //在32bit的arm指令集中，stubcode中的4条指令占用16个字节的空间
    //前两条指令为hook点被替换的两条指令
    //后两条指令跳转回原程序
    void *stubcode = malloc(16);
    memcpy(stubcode, (void *) hook_addr, 8);
    //ldr pc, [pc, #-4]
    //[address]
    //手动填充stubcode
    char jump_ins[8] = {0x04, 0xF0, 0x1F, 0xE5};
    uint32_t jmp_address = hook_addr + 8;
    memcpy(jump_ins + 4, &jmp_address, 4);
    memcpy(stubcode + 8, jump_ins, 8);
    //添加执行属性
    change_addr_writable((long) stubcode, true);
    //修复shellcode中的两个地址值
    uint32_t *shell_hook = shellcode + (p_hook_func - p_shellcode_start);
    *shell_hook = (uint32_t) func;
    uint32_t *shell_stub = shellcode + (p_stub_func - p_shellcode_start);
    *shell_stub = (uint32_t) stubcode;
    //为hook点添加写属性
    change_addr_writable(hook_addr, true);
    //替换hook点指令为跳转指令，跳转至shellcode
    jmp_address = (uint32_t) shellcode;
    memcpy(jump_ins + 4, &jmp_address, 4);
    memcpy((void *) hook_addr, jump_ins, 8);
    change_addr_writable(hook_addr, false);
    //刷新cache
    cacheflush(hook_addr, 8, 0);
    return true;
}