#include "types.h"

typedef int bool;
#define NULL ((void *)0)
extern u8 Data_03001810[];

s32 Func_08096c24(void) {
    u8 *cursor = Data_03001810;
    s32 count = 0;
    s32 remaining = 0x200;
    do {
        if (*cursor++ == 0xff)
            count += 1;
        remaining -= 1;
    } while (remaining != 0);
    return count;
}
