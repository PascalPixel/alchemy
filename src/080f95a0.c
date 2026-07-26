#include "types.h"

typedef int bool;
#define NULL ((void *)0)

s32 Func_080030f8(s32);

void Func_080f95a0(void) {
    s32 count = 0;
    do {
        if (*(u8 *)0x02003000 == 0) {
            break;
        }
        Func_080030f8(1);
        count++;
    } while (count <= 299);
}
