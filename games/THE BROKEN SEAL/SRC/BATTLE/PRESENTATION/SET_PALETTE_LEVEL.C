/* BattlePresentation_SetPaletteLevel: with IME off, copy the battle
   palette to BG palette 6 (level 0) or darken it by level/60 into the
   same place, recording the scale. FAKEMATCH: the IME save sits in a
   one-pass loop around a pointer to its stack slot (taken before the IME
   register address), and the scale is assigned inside the call's argument
   list; the u32 restore temporary loads the saved word before the IME
   address. */
#include "TYPES.H"
#include "DMA.H"

struct BattleScreen {
    u8 unknown_000[0x544];
    u16 palette[128];
    s32 brightness;
};

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);

void BattlePresentation_SetPaletteLevel(s32 unused, s32 level)
{
    struct BattleScreen *screen = *(struct BattleScreen **)0x03001e74;
    u16 *palette = screen->palette;
    volatile u32 ime;

    do {
        volatile u32 *slot = &ime;
        u16 *ime_reg = (u16 *)0x04000208;

        do {
            *slot = *ime_reg;
            *ime_reg = (u32)ime_reg;
        } while (0);
    } while (0);

    if (level == 0) {
        Dma_Set(palette, (void *)0x050000c0, 0x80000080, (volatile u32 *)0x040000d4);
    } else {
        Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0, screen->brightness = 0x10000 - level * 1092, 128);
    }
    {
        u32 saved = ime;

        *(volatile u16 *)0x04000208 = saved;
    }
}
