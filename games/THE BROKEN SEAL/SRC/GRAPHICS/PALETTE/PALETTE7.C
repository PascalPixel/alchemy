#include "TYPES.H"
#include "DMA.H"
#include "SCENE.H"

extern u8 gBattleFxWork[];

void BattleEffect_RunParticleStreams(s32, s32);

/* Brighten background palette entries 1-63 by per-channel deltas, clamping
   each 5-bit channel at 31. */
void Palette_BrightenBgEntries(s32 blue_delta, s32 green_delta, s32 red_delta)
{
    u16 *color = (u16 *)0x05000002;
    s32 i;

    for (i = 0; i != 63; i++) {
        s32 blue = (*color >> 10) & 0x1f;
        s32 green = (*color >> 5) & 0x1f;
        s32 red = *color & 0x1f;

        blue += blue_delta;
        green += green_delta;
        red += red_delta;
        if (blue > 31)
            blue = 31;
        if (green > 31)
            green = 31;
        if (red > 31)
            red = 31;
        *color++ = (blue << 10) | (green << 5) | red;
    }
}

/* H-blank callback: feed the per-line backdrop colours in the battle work
   area to palette entry 0. */
void BattleFx_ArmPaletteHBlankDma(void)
{
    u8 *work = *(u8 **)gBattleFxWork;
    volatile u16 *channel = (volatile u16 *)0x040000b0;
    channel[5] &= 0xc5ff;
    channel[5] &= 0x7fff;
    (void)channel[5];
    Dma_Set(work + 0x1f80, (void *)0x05000000, 0xa2600001, (volatile u32 *)channel);
}

/* battle/effects/runtime/initialize_default_mode.c */
void BattleFx_InitializeDefaultMode(s32 arg0)
{
    BattleEffect_RunParticleStreams(arg0, 0);
}

/* battle/effects/runtime/initialize_mode_1.c */
void BattleFx_InitializeMode1(s32 arg0)
{
    BattleEffect_RunParticleStreams(arg0, 1);
}
