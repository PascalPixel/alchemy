#include "DMA.H"

extern u16 gBgScroll[];
extern s32 gProjection[];


void WaitFrames(s32 frames);
void BattleBackground_LoadFar(s32 mode, s32 value, s32 size);

/* Switches to mode 1, clears the effect buffer and the second character
   block, then enables BG0-BG2 and OBJ with alpha blending (EVA 14,
   EVB 16) and arms a 120-frame timer. */
void BattleEffect_SetupBlendedDisplay(void)
{
    u8 **pointers = (u8 **)0x03001e74;
    u8 *buffer = pointers[31];
    u8 *work = pointers[0];
    volatile u32 zero;

    *(u16 *)0x04000000 = 1;
    gBgScroll[3] = 32;
    BattleBackground_LoadFar(1, *(u16 *)(work + 0x648), 24);
    zero = 0;
    Dma_Set((const void *)&zero, buffer, 0x85001000, (volatile u32 *)0x040000d4);
    zero = 0;
    Dma_Set((const void *)&zero, (void *)0x06004000, 0x85001000, (volatile u32 *)0x040000d4);
    WaitFrames(1);
    *(u16 *)0x04000052 = 0x100e;
    *(u16 *)0x04000050 = 0x3f46;
    *(u16 *)0x04000000 = 0x7741;
    gProjection[4] = 120;
}
