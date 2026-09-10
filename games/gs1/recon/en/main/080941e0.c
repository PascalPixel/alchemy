#include "types.h"

extern s16 Data_02000240[];

void Func_080030f8(s32 frames);
void Func_080901c0(s32, s32);
void Func_080f9010(s32 cue);

void Func_080941e0(void)
{
    u8 *state = *(u8 **)0x03001ebc;
    s32 i;
    s32 level;
    s32 colour;

    Func_080f9010(Data_02000240[247]);
    Func_080f9010(0x120);
    Func_080f9010(147);
    if (*(s16 *)(state + 0x19e) == 3) {
        *(volatile u16 *)0x050001e6 = 0x7fff;
        Func_080901c0(0x401, 16);
        *(u16 *)(state + 0x1c6) = 0;
        Func_080030f8(16);
        for (i = 0; i < 16; i++) {
            level = 15 - i;
            colour = (level << 11) | (level << 6) | (level << 1);
            *(volatile u16 *)0x050001e6 = colour;
            Func_080030f8(1);
        }
    } else {
        *(volatile u16 *)0x05000000 = 0x7fff;
        Func_080901c0(0x207, 16);
        *(u16 *)(state + 0x1c6) = 0;
        Func_080030f8(16);
        for (i = 15; i >= 0; i--) {
            *(volatile u16 *)0x05000000 = (i << 11) | (i << 6) | (i << 1);
            Func_080030f8(1);
        }
    }
}
