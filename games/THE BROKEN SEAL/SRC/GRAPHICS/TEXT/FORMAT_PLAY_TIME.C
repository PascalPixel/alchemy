#include "TYPES.H"

u32 Math_DivU(s32, s32);
s32 Math_ModU(s32, s32);
u8 *UiText_FormatNumber(u8 *, s32, s32);

void *Text_FormatPlayTime(s32 value, u8 *out)
{
    u8 buf[64];
    u32 time;
    u32 minutes;
    s32 seconds;
    u8 *s;
    u8 *p;

    time = Math_DivU(value, 0xe10);
    if (time > 0xea5f)
        time = 0xea5f;

    minutes = Math_DivU(time, 60);
    seconds = Math_ModU(time, 60);

    s = UiText_FormatNumber(buf, minutes, 3);
    *out = *s;
    s++;
    p = out + 1;
    *p = *s;
    p++;
    *p = s[1];
    seconds += 100;
    p++;
    *p = ':';

    s = UiText_FormatNumber(buf, seconds, 2);
    p++;
    *p = s[0];
    p++;
    *p = s[1];
    p[1] = 0;

    return out;
}
