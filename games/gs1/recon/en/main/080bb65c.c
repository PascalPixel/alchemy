#include "types.h"

#define BattlePresentation_WaitForAdvance Func_080bb65c

extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001c94;

void BattlePresentation_WaitForAdvance(void)
{
    u8 prompt[12];
    s32 frames;
    s32 sprite;

    while (Func_08015048() == 0) {
        Func_080030f8(1);
    }

    sprite = Func_080040b4(0x80);
    frames = 0;
    for (;;) {
        u32 phase = (*(u32 *)0x03001e40 >> 2) & 7;
        const void *tiles = (const void *)(0x080c3734 + phase * 128);
        u8 **render = *(u8 ***)0x03001ee4;
        u8 *left = render[0];
        u8 *right = render[1];
        s32 wave;

        Func_080039fc((void *)0x0400004a, 4);
        Func_0800393c((void *)0x0400004a, 16);
        *(u16 *)0x04000052 = 16;
        *(u32 *)(prompt + 4) = 0xa400;
        *(u32 *)(prompt + 8) = 0;
        *(u16 *)(prompt + 8) =
            (*(u16 *)(prompt + 8) & 0xfc00) |
            (Func_080040d0(sprite, tiles) & 0x03ff);
        *(u16 *)(prompt + 6) =
            (*(u16 *)(prompt + 6) & 0xfe00) |
            (((*(u16 *)(left + 12) * 8) +
              (*(u16 *)(right + 4) >> 8) + 4) & 0x01ff);
        wave = Func_08002322(*(u32 *)0x03001e40 << 12);
        if (wave < 0) {
            wave += 0x7fff;
        }
        prompt[4] = (*(u16 *)(right + 6) >> 8) +
            (wave >> 15) + (*(u16 *)(left + 14) * 8) + 6;
        Func_08003dec(prompt, 240);

        if (Data_03001ae8 & 2) {
            break;
        }
        if (Data_03001c94 & 0x303) {
            break;
        }
        if (frames > 15 && (Data_03001ae8 & 0x303)) {
            break;
        }
        Func_080030f8(1);
        frames++;
    }

    Func_080f9010(0x6f);
    Func_08003f3c(sprite);
    Func_080030f8(1);
}
