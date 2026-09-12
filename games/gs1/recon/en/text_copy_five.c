#include "types.h"
#include "scene.h"


u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source)
{
    u8 buffer[5];
    u32 fill = '_';
    u8 *p = buffer;
    u8 *dst = p;
    u8 *base = p;
    s32 cnt = 3;

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
