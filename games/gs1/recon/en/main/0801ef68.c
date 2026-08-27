#include "types.h"

extern u8 *Data_03001e8c;
extern const s8 Data_080371c4[];

struct BarWindow {
    u8 pad[8];
    u16 count;
    u16 segments;
    u16 y;
    u16 x;
};

void Func_0801ef68(struct BarWindow *window, u32 flags)
{
    u8 *base = Data_03001e8c;
    s32 max = window->count - 1;
    s32 segments = window->segments;
    s32 startIndex = 0;
    s32 mode = 0;
    const s8 *string;
    s32 i;

    if ((flags & 1) == 0)
        flags &= ~3;
    if (flags & 2) {
        mode = 5;
        startIndex = 0;
    }

    string = Data_080371c4;
    i = startIndex;

    for (;;) {
        s32 offset;
        s8 ch = string[i];

        if (ch < 0)
            break;

        offset = ch + mode;
        if (offset < max && segments != 0) {
            s32 j;

            for (j = 0; j < segments; j++) {
                u16 *dst = (u16 *)(base + (window->y + j) * 64 + window->x * 2 + offset * 2);
                u16 value;

                if (j == 0)
                    value = 0xF018;
                else if (j == segments - 1)
                    value = 0xF00F;
                else
                    value = 0xF00F;

                *dst = value;
            }
        }
        i++;
    }

    if (base[0xea5] != 0) {
        u16 *dst = (u16 *)(base + ((window->y + window->segments) << 6) +
            (window->x << 1) - 64);
        s32 n;

        *dst = 0xF080;
        dst++;
        for (n = 1; n < max; n++) {
            *dst = 0xF081;
            dst++;
        }
        *dst = 0xF082;
    }

    base[0xea3] = 1;
}
