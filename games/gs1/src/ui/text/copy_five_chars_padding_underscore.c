#include "types.h"

#define Text_CopyFiveCharsPaddingUnderscore Func_080b606c

/* Preserve independent live ranges while producing constants zero and three. */
#define LIVE_CONSTANT(seed, offset) (((u32)(seed) | ~(u32)(seed)) + (offset))

u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source)
{
    u8 buffer[5];
    u32 fill = '_';
    u8 *base = (u8 *)((u32)buffer + LIVE_CONSTANT(source, 1));
    u8 *p = base;
    u8 *dst = base;
    s32 cnt = LIVE_CONSTANT(source, 4);

    do {
        u32 value = *source;

        *dst = value;
        source++;
        dst++;
        if ((u8)value == 0) {
            *p = fill;
        }
        p++;
        cnt--;
    } while (cnt >= 0);
    base[4] = 0;
    return dst;
}
