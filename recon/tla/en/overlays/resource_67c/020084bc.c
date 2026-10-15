/* Draft of Func_020004bc (overlay 67C, 184 bytes), differ 21 of 72: the native
   shifts the column into r3 and copies it to r7 (asrs r3, r3, #20; adds r7, r3),
   takes 42 off it only after the three calls that follow, and keeps the layer
   in r6; this form shifts straight into r6, subtracts at once and keeps the
   layer in r7. Measured: column - 42 written inside the loop, with or without
   an empty asm barrier before the loop, is differ 31. Func_02000574, the same
   event without the column, matches (s0574.c). */
#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_0200349c(s32 column, s32 value);

void Func_020004bc(s32 unused, s32 id)
{
    u8 *map = (u8 *)Ram_HeapSlots->mapWork;
    s32 *layer;
    s32 i;
    s32 column;

    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native loads the map work before it copies the id (differ 2 without it). */
    layer = (s32 *)(map + 0x140);
    Object_SetPartAttribute(Object_GetById(id), 0);
    Func_02001e78(id);
    Object_SetModeById(id, 3);
    layer[3] -= 0x200000;
    Func_02003d54();
    Func_02003cdc();
    column = Object_GetById(12)->x >> 20;
    Func_02003d7c();
    Func_02003eac(0);
    Func_02003ef4(0x22c);
    column -= 42;
    for (i = 0; i <= 31; i++) {
        Func_02003460(i << 16);
        Func_0200349c(column, 0);
        layer[3] += 0x10000;
        WaitFrames(4);
    }
    if (GameFlag_Test(0x250))
        Func_02003d1c_(41, 42, 1, 1, 43, 44);
    Func_02003d84();
    GameFlag_SetBit(0x230);
}
