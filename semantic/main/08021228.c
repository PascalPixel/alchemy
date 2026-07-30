#include "types.h"

s32 Func_080162d4(u32, u32, u32, u32, s32);
void Func_0801e41c(s32, s32, s32, s32, s32);
u32 Func_08019d2c(s16);
void Func_0801a4fc(u32, s32, s32 *, s32 *, s32, s32);
void Func_08019908(u32, u32);
s32 Func_08019ba0(s32);
void Func_080165d8(s32, s32, s32, s32, s32);
void Func_080f9010(s32);
void Func_08003dec(s32 *, s32);
void Func_080030f8(u32);
s32 Func_080f9048(void);
void Func_08016418(s32, s32);
void Func_08003f3c(u32);

void Func_08021228(u32 resource, s32 character, s32 base)
{
    u8 *state = *(u8 **)0x03001e8c;
    s16 portrait = *(s16 *)(0x080371fe + (character & 3) * 2);
    s32 object = Func_080162d4(2, 1, 0x1a, 5, 0);
    s32 graphics;
    s32 palette;
    s32 transition[3];

    if (object == 0)
        return;

    Func_0801e41c(object, 4, 0, 4, 4);
    state[0xea3] = 1;
    Func_0801a4fc(
        Func_08019d2c(portrait), 0, &graphics, &palette, 0x0e, 0);
    transition[0] = 0;
    transition[1] = 0x8014000c;
    transition[2] = palette | 0xe000;
    *(u16 *)(state + 0x12f4) = 0;
    *(u16 *)(state + 0x12f6) = 0;

    Func_08019908(resource, 1);
    Func_08019908(character * 20 + base + 0x12c, 4);
    Func_080165d8(
        object, Func_08019ba0(character + 0x980), 0x24, 2, 0);
    Func_080f9010(0x51);

    do {
        Func_08003dec(transition, 0xfa);
        Func_080030f8(1);
    } while (Func_080f9048() != 0 &&
             !(*(s32 *)0x03001c94 & 0x303));

    Func_08016418(object, 2);
    Func_080030f8(1);
    Func_08003f3c(graphics);
}
