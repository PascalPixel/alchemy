#include "types.h"

#define REG32(address) (*(volatile u32 *)(address))

void Func_08003538(void)
{
    s32 counter = REG32(0x03001b00);
    u32 flags;
    u32 selected = 0;
    u32 count;
    u32 mask;
    volatile u32 *active;

    if (counter <= 0) {
        REG32(0x03001b04) = REG32(0x03001ae8);
        flags = REG32(0x03001b04);
        if (counter == 0)
            REG32(0x03001b00) = 6;
        else
            REG32(0x03001b00) = 19;
    } else {
        REG32(0x03001b04) = 0;
        flags = REG32(0x03001b04);
    }

    if (flags != 0) {
        count = 0;
        if ((flags & 0x40) != 0)
            count = 1;
        if ((flags & 0x80) != 0)
            count++;
        if ((flags & 0x20) != 0)
            count++;
        if ((flags & 0x10) != 0)
            count++;

        active = (volatile u32 *)0x03001afc;
        *active = flags;
        switch (count) {
        default:
            REG32(0x03001d04) = 0x30;
            mask = 0xff0f;
            *active &= mask;
            break;
        case 0:
            REG32(0x03001d04) = 0x30;
            break;
        case 1:
            REG32(0x03001d04) = flags & 0xf0;
            break;
        case 2:
            if ((REG32(0x03001d04) & *active) == 0)
                REG32(0x03001d04) = 0x30;
            *active &= REG32(0x03001d04) ^ 0xffff;
            break;
        case 3:
            if ((REG32(0x03001d04) & 0x30) != 0)
                selected = 0x30;
            if ((REG32(0x03001d04) & 0xc0) != 0)
                selected = 0xc0;
            mask = 0xffff ^ selected;
            REG32(0x03001d04) = flags & mask;
            *active &= mask;
            break;
        }
    } else {
        REG32(0x03001afc) = flags;
    }

    REG32(0x03001c94) =
        (REG32(0x03001ae8) ^ REG32(0x03001cf4)) & REG32(0x03001ae8);
    REG32(0x03001cf4) = REG32(0x03001ae8);
}
