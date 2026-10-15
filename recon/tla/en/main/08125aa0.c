/* Near miss: differ 6 of 94 (three reordered rows, all in the opening; differ 8 before the one-pass block on the saved word). The native builds the BG0CNT address and reads it ahead of the prologue sub sp and of the address of the first saved slot (movs r2; lsls r2; sub sp; adds r2; movs r0, #6; ldrh; add r0, sp); the draft has sub sp and the slot address first. The opening block holds the prologue, so no block can stand there. */
#include "fixed_math.h"
#include "types.h"
#include "system.h"

void Fade_ToBlack(s32 arg0);

s32 BattlePres_RunMosaicFadeOut(void)
{
    volatile u16 bg0cnt;
    volatile u16 bg1cnt;
    volatile u16 bg2cnt;
    volatile u16 bg3cnt;
    s32 i;
    volatile u16 *reg;
    u32 saved;

    bg0cnt = *(volatile u16 *)0x04000008;
    *(volatile u16 *)0x04000008 = bg0cnt | 0x40;
    reg = (volatile u16 *)0x04000008;
    bg1cnt = *++reg;
    *reg = bg1cnt | 0x40;
    bg2cnt = *++reg;
    *reg = bg2cnt | 0x40;
    bg3cnt = *++reg;
    *reg = bg3cnt | 0x40;
    reg += 0x21;
    *reg = 0x3eee;

    Fade_ToBlack(16);
    for (i = 0; i <= 15; i++) {
        Random16();
        Random16();
        Random16();
        Random16();
        *(volatile u16 *)0x0400004c = (i << 8) | i;
        WaitFrames(1);
    }

    *(volatile u16 *)0x04000000 = 1;
    WaitFrames(4);
    /* FAKEMATCH: the one-pass block ends the scheduling block: the native reads the saved control word before it builds the register address (one reordered row). */
    do {
        saved = bg0cnt;
    } while (0);
    reg = (volatile u16 *)0x04000008;
    *reg++ = saved;
    *reg++ = bg1cnt;
    *reg++ = bg2cnt;
    *reg = bg3cnt;
    return 0;
}
