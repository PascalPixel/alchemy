#include "types.h"

void Func_080c0228(void) {
    s32 value = **(s32 **)0x03001EF8;

    if ((u32)value <= 79) {
        s32 row;
        u32 tile;
        u16 *destination;
        s32 count;
        u32 screen_base;

        tile = (value & 7) + 0xF081;
        row = value;
        if (row < 0)
            row += 7;
        row = 13 - (row >> 3);
        screen_base = 0x06006000;
        destination = (u16 *)(screen_base + (row << 6));
        count = 0;
        do {
            count++;
            *destination++ = tile;
        } while (count != 32);

        tile |= 0x800;
        row = value;
        if (row < 0)
            row += 7;
        row = (row >> 3) + 13;
        if ((u32)row <= 20) {
            count = 0;
            destination = (u16 *)(0x06006000 + (row << 6));
            do {
                count++;
                *destination++ = tile;
            } while (count != 32);
        }
    }
}

void Func_080c0294(void) {
    u32 zero = 0;
    volatile u16 *display_control = (volatile u16 *)0x04000012;

    *display_control = zero;
}
