#include <stdio.h>

#include "include/inlineHook.h"
#include "hooktest.h"
#include "log.h"

int (*old_puts)(const char *) = NULL;

int new_puts(const char *string)
{
    LOG_DEBUG("inlineHook success");
    old_puts("inlineHook success");
    return 0;
}

int hook()
{
    if (registerInlineHook((uint32_t) puts, (uint32_t) new_puts, (uint32_t **) &old_puts) != ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) puts) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int unHook()
{
    if (inlineUnHook((uint32_t) puts) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int test()
{
    puts("test");
    hook();
    puts("test");
    unHook();
    puts("test");
    return 0;
}