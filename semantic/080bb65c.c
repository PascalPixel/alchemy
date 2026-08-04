#include "types.h"

void Func_080030f8(u32);
s32 Func_08015048(void);
s32 Func_080040b4(s32);
void Func_080039fc(s32, s32);
void Func_0800393c(s32, s32);
s32 Func_080040d0(s32, s32);
s32 Func_08002322(s32);
void Func_08003dec(void *, s32);
void Func_080f9010(s32);
void Func_08003f3c(u32);

void Func_080bb65c(void)
{
    s32 graphics;
    s32 frames = 0;
    u8 sprite[12];

    while (Func_08015048() == 0)
        Func_080030f8(1);
    graphics = Func_080040b4(0x80);

    for (;;) {
        s32 phase = *(s32 *)0x03001e40;
        u8 *pair = *(u8 **)0x03001ee4;
        u8 *first = *(u8 **)(pair + 0);
        u8 *second = *(u8 **)(pair + 4);
        s32 tile_source =
            0x080c3734 + (((u32)phase >> 2) & 7) * 0x80;
        s32 sine;
        u16 x;

        Func_080039fc(0x0400004a, 4);
        Func_0800393c(0x0400004a, 0x10);
        *(volatile u16 *)0x04000052 = 0x10;
        *(u32 *)(sprite + 4) = 0xa400;
        *(u32 *)(sprite + 8) = 0;
        *(u16 *)(sprite + 8) =
            (*(u16 *)(sprite + 8) & 0xfc00) |
            (Func_080040d0(graphics, tile_source) & 0x3ff);

        x = (*(u16 *)(first + 0x0c) * 8 +
             (*(u16 *)(second + 4) >> 8) + 4) & 0x1ff;
        *(u16 *)(sprite + 6) =
            (*(u16 *)(sprite + 6) & 0xfe00) | x;

        sine = Func_08002322(phase << 12);
        if (sine < 0)
            sine += 0x7fff;
        sprite[4] =
            (*(u16 *)(second + 6) >> 8) +
            (sine >> 15) +
            *(u16 *)(first + 0x0e) * 8 +
            6;
        Func_08003dec(sprite, 0xf0);

        if ((*(s32 *)0x03001ae8 & 2) ||
            (*(s32 *)0x03001c94 & 0x303) ||
            (frames > 15 && (*(s32 *)0x03001ae8 & 0x303))) {
            break;
        }
        Func_080030f8(1);
        frames++;
    }

    Func_080f9010(0x6f);
    Func_08003f3c(graphics);
    Func_080030f8(1);
}
