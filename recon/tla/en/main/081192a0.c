/*
 * Draft: Text_CopyFiveCharsPaddingUnderscore does not yet match; 3 halfwords differ from ☀️'s C, first at +0x10 (adds r2, #2).
 * Links as recon/tla/raw/08119054.s.
 */
#include "TYPES.H"

u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source)
{
    u8 buffer[5];
    u8 *base = buffer;
    u32 fill = '_';
    u8 *p = base;
    u8 *dst = p;
    s32 count = 3;

    do {
        u32 value = *source;

        *dst = value;
        source++;
        dst++;
        if ((u8)value == 0)
            *p = fill;
        p++;
        count--;
    } while (count >= 0);
    base[4] = 0;
    return dst;
}
