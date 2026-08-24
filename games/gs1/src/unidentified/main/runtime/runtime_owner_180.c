#include "types.h"

extern u8 *Data_03001f2c;

s32 Func_080b27b0(s32 selection, s32 variant);

s32 Func_080b280c(void)
{
    u8 *work = Data_03001f2c;
    u8 *base;
    s32 active = 0;
    s32 variant = (s8)work[0x3AA];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(work + 0x3A7)) {
        base = work + 2;
        offset = 0x36C;
        do {
            if (Func_080b27b0(*(s16 *)(base + offset), variant) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(work + 0x3A7));
    }

    return active;
}
