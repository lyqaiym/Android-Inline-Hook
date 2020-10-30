#include "target.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "log.h"

void target_foo() {
//    int a = 3;
//    int b = 2;
    int c = 1;
//    while (a--) {
//        sleep(2);
//        b = a * b;
////        printf("[INFO] b is %d\n", b);
//        LOG_DEBUG("[INFO] b is %d\n", b);
//    }
//    b = b + 2;
//    b = b - 1;
//    abort();
//    printf("[INFO] finally, b is %d\n", b);
//    LOG_DEBUG("[INFO] finally, b is %d, c is %d\n", b, c);
    LOG_DEBUG("[INFO] finally, c1 is %d\n", c);
    c = 2;
    LOG_DEBUG("[INFO] finally, c2 is %d\n", c);
    c = 3;
    LOG_DEBUG("[INFO] finally, c3 is %d\n", c);
}