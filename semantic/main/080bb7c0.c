#include "types.h"

void Func_080030f8(u32);
s32 Func_08015048(void);
s32 Func_080153f0(s32);
s32 Func_080040b4(s32);
void Func_080039fc(s32, s32);
void Func_0800393c(s32, s32);
s32 Func_080040d0(s32, s32);
void Func_08003dec(void *, s32);
void Func_08003f3c(u32);

void Func_080bb7c0(s32 base_x, s32 base_y)
{
    s32 reference = Func_080153f0(0);
    s32 graphics;
    u16 x_offset = base_x;
    u8 sprite[12];

    while (Func_08015048() == 0)
        Func_080030f8(1);

    graphics = Func_080040b4(0x80);
    for (;;) {
        s32 shifted = *(s32 *)0x03001e40 & 4;
        u16 tile;
        u16 x;

        Func_080039fc(0x0400004a, 4);
        Func_0800393c(0x0400004a, 0x10);
        *(volatile u16 *)0x04000052 = 0x10;
        *(u32 *)(sprite + 4) = 0x40000000;
        *(u32 *)(sprite + 8) = 0;

        tile = Func_080040d0(graphics, reference) & 0x3ff;
        *(u16 *)(sprite + 8) =
            (*(u16 *)(sprite + 8) & 0xfc00) | tile;
        x = ((shifted >> 1) + x_offset - 4) & 0x1ff;
        *(u16 *)(sprite + 6) =
            (*(u16 *)(sprite + 6) & 0xfe00) | x;
        sprite[4] = base_y - (shifted >> 2) + 0xf8;
        Func_08003dec(sprite, 0xf0);

        if (*(s32 *)0x03001c94 & 0x303)
            break;
        Func_080030f8(1);
    }

    Func_08003f3c(graphics);
    Func_080030f8(1);
}
