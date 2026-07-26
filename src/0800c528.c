#include "types.h"

typedef int bool;
#define NULL ((void *)0)
void Func_0800c528(u8 *arg0, u32 arg1) {
    if (arg0 != NULL) {
        if ((arg0[0x54] & 0xf) == 1)
            (*(u8 **)(arg0 + 0x50))[0x26] = arg1;
    }
}
