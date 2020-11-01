#include "target.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "log.h"

void target_foo() {
    int a = 22;
    int c = 12345;
    LOG_DEBUG("[INFO] finally, c1 is %d,%p\n", c, &c);
    if (c > a) {
        LOG_DEBUG("[INFO] finally, c2 is %d,%p\n", c, &c);
    } else {
        //hook 以后走这
        LOG_DEBUG("[INFO] finally, c3 is %d,%p\n", c, &c);
    }
}