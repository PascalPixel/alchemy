#include "types.h"

extern volatile u32 Data_03001c94;

void *Func_080153f0(s32 mode);
s32 Func_08015048(void);
void Func_080030f8(s32 frames);
s32 Func_080040b4(s32 size);
void Func_080039fc(void *reg, s32 val);
void Func_0800393c(void *reg, s32 val);
s32 Func_080040d0(s32 sprite, void *tiles);
void Func_08003dec(void *prompt, s32 priority);
void Func_08003f3c(s32 sprite);

void Func_080bb7c0(s32 x, s32 y)
{
    u8 prompt[12];
    s32 sprite;
    void *tiles = Func_080153f0(0);

    while (Func_08015048() == 0) {
        Func_080030f8(1);
    }

    sprite = Func_080040b4(0x80);

    for (;;) {
        Func_080039fc((void *)0x0400004a, 4);
        Func_0800393c((void *)0x0400004a, 16);
        *(u16 *)0x04000052 = 16;
        *(u32 *)(prompt + 4) = 0x40000000;
        *(u32 *)(prompt + 8) = 0;
        *(u16 *)(prompt + 8) =
            (*(u16 *)(prompt + 8) & 0xfc00) |
            (Func_080040d0(sprite, tiles) & 0x03ff);
        *(u16 *)(prompt + 6) =
            (*(u16 *)(prompt + 6) & 0xfe00) |
            ((((*(u32 *)0x03001e40 & 4) >> 1) + (u16)x + 0xfffc) & 0x01ff);
        prompt[4] = (u8)(y - ((*(u32 *)0x03001e40 & 4) >> 2) + 248);
        Func_08003dec(prompt, 240);

        if (Data_03001c94 & 0x303) {
            break;
        }
        Func_080030f8(1);
    }

    Func_08003f3c(sprite);
    Func_080030f8(1);
}
