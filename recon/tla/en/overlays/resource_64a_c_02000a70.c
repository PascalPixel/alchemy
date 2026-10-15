/* Near miss of the Japanese Func_02000a70, differ 4 of 46 (the other five
 * editions are C in games/tla/src/field/overlay_64a/s0a70.c): in both dakuten
 * branches the native loads the 0x4000 mark (ldr r2) before it copies the
 * cell (adds r3, r0, #0); this draft copies first. Tried: the mark first in
 * the OR, a u16 or u8 cast of the sum, a u16 sum local (17), a u32 or u16
 * mark local behind an empty asm (34, 47), one-pass blocks, three minutes of
 * alchemy permute (2 at best, by storing the mark and ORing the stored
 * halfword, two stores). */
#include "types.h"

void Func_02000a70(s32 cell, u16 *text)
{
    text[1] = 0;
    text[2] = 0;
    if (cell <= 14)
        text[0] = cell + 145;
    else if (cell <= 43)
        text[0] = cell + 209;
    else if (cell == 45)
        text[0] = 0x86;
    else if (cell == 44)
        text[0] = 0xfd;
    // The voiced kana carry the dakuten mark in the high byte.
    else if (cell <= 55)
        text[0] = (cell + 104) | 0x4000;
    else if (cell == 56)
        text[0] = 0xe0 | 0x4000;
    else if (cell == 57)
        text[0] = 0xe3 | 0x4000;
    else if (cell == 58)
        text[0] = 0xe4 | 0x4000;
    else
        text[0] = (cell + 175) | 0x4000;
}
