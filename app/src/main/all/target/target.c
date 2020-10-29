#include "target.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "log.h"

void target_foo() {
    int a = 3;
    int b = 2;
    while (a--) {
        sleep(2);
        b = a * b;
//        printf("[INFO] b is %d\n", b);
        LOG_DEBUG("[INFO] b is %d\n", b);
    }
    b = b + 2;
    b = b - 1;
//    printf("[INFO] finally, b is %d\n", b);
    LOG_DEBUG("[INFO] finally, b is %d\n", b);
}