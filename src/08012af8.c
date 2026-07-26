#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

u8 *Func_08185000(s32 arg0);

s32 Func_08012af8(s32 arg0, s32 arg1) {
    s32 v = arg0;
    s32 d = arg1 + v - v;
    for (;;) {
        v += d;
        if (v < 0) {
            v = 0x200;
            continue;
        }
        {
            s32 lim = 0x200;
            if (v >= lim) {
                v = -1;
                continue;
            }
        }
        if (*Func_08185000(v)) {
            return v;
        }
    }
}
