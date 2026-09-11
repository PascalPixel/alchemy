#include "types.h"
#include "scene.h"
#include "abi/ui/text/fmt/format_play_time.h"

u8 *Text_FormatNumber(u8 *, s32, s32);

void *Text_FormatPlayTime(s32 value, u8 *out)
{
    u8 buf[64];
    u32 time;
    u32 minutes;
    s32 seconds;
    u8 *s;
    u8 *p;

    time = Ui_Apply(value, 0xe10);
    if (time > 0xea5f)
        time = 0xea5f;

    minutes = Ui_Apply(time, 60);
    seconds = Ui_Apply2(time, 60);

    s = Text_FormatNumber(buf, minutes, 3);
    *out = *s;
    s++;
    p = out + 1;
    *p = *s;
    p++;
    *p = s[1];
    seconds += 100;
    p++;
    *p = ':';

    s = Text_FormatNumber(buf, seconds, 2);
    p++;
    *p = s[0];
    p++;
    *p = s[1];
    p[1] = 0;

    return out;
}
