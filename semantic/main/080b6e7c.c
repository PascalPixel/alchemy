#include "types.h"

/*
 * ROM 0x080b6e7c..0x080b6eb4: complete table-search leaf, reviewed from
 * out/decomp/diagnose/080b6e7c/080b6e7c.s and its direct caller
 * src/080b6eb4.c.  Table entries carry a 9-bit key and a 7-bit result; the
 * signed 0xffff entry terminates a miss.  asm/080b6e7c.s retains byte identity
 * while this file expresses the behavior without compiler escape hatches.
 */
extern const u16 Data_080c593c[];

u32 Func_080b6e7c(s32 key)
{
    s32 index = 0;

    for (;;) {
        u16 entry = Data_080c593c[index];
        if (key == (entry & 0x1ff)) return entry >> 9;
        if ((s16)entry == -1) return 6;
        index++;
    }
}
