#include "PROJECT.H"
#include "DMA.H"
#include "IO_REG.H"

extern u8 gBattleWork[];

extern u16 gBgScroll[];



void WaitFrames(s32 frames);
void BattleBackground_LoadFar(s32 mode, s32 value, s32 size);

/* Switches to mode 1, clears the effect buffer and the second character
   block, then enables BG0-BG2 and OBJ with alpha blending (EVA 14,
   EVB 16) and arms a 120-frame timer. */
void BattleEffect_SetupBlendedDisplay(void)
{
    u8 **pointers = (u8 **)gBattleWork;
    u8 *buffer = pointers[31];
    u8 *work = pointers[0];
    volatile u32 zero;

    REG_DISPCNT = 1;
    gBgScroll[3] = 32;
    BattleBackground_LoadFar(1, *(u16 *)(work + 0x648), 24);
    zero = 0;
    Dma_Set((const void *)&zero, buffer, 0x85001000, REG_DMA3);
    zero = 0;
    Dma_Set((const void *)&zero, (void *)0x06004000, 0x85001000, REG_DMA3);
    WaitFrames(1);
    REG_BLDALPHA = 0x100e;
    REG_BLDCNT = 0x3f46;
    REG_DISPCNT = 0x7741;
    gProjection.center_y = 120;
}
