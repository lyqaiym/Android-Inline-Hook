#include <stdio.h>

#include "include/inlineHook.h"
#include "hooktest.h"
#include "log.h"

int (*old_puts)(const char *) = NULL;

int new_testchar(const char *string) {
    LOG_DEBUG("new_testchar:char=%s", string);
    old_puts("inlineHook success");
    return 0;
}

void testchar(const char *__s) {
    LOG_DEBUG("testchar:s=%s", __s);
}

int hook() {
    if (registerInlineHook((uint32_t) testchar, (uint32_t) new_testchar, (uint32_t **) &old_puts) !=
        ELE7EN_OK) {
        return -1;
    }
    if (inlineHook((uint32_t) testchar) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int unHook() {
    if (inlineUnHook((uint32_t) testchar) != ELE7EN_OK) {
        return -1;
    }

    return 0;
}

int test() {
    testchar("test1");
    hook();
    testchar("test2");
    unHook();
    testchar("test3");
    return 0;
}