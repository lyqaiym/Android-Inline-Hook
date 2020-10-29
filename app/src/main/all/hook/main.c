#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <stdbool.h>
#include "hook_inline.h"
#include "log.h"

typedef void (*target_foo)(void);

void my_func(struct hook_reg *reg)
{
    LOG_DEBUG("my_func");
    puts("here we go!");
}

void test()
{
    void *handler = dlopen("libtarget.so", RTLD_NOW);
    target_foo foo = (target_foo)dlsym(handler, "target_foo");
    hook_inline_make("libtarget.so", 0xde2, my_func, true);
    foo();
}