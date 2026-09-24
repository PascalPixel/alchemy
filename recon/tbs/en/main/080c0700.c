/* 2026-09-24: rewritten, 57 differing halfwords (112 of 116 bytes). The IME
   save stores the register address as its own value (strh r3,[r3]); the
   reference keeps the battle screen in r0, the palette pointer in r5, the
   level in r4 and the saved IME slot address in r6. */
#include "TYPES.H"
#include "DMA.H"

struct BattleScreen {
    u8 unknown_000[0x544];
    u16 palette[128];
    s32 brightness;
};

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);

void Func_080c0700(s32 unused, s32 level)
{
    struct BattleScreen *screen = *(struct BattleScreen **)0x03001e74;
    u16 *palette = screen->palette;
    volatile u32 ime;

    { volatile u16 *reg = (volatile u16 *)0x04000208; ime = *reg; *reg = (u32)reg; }
    if (level == 0) {
        Dma_Set(palette, (void *)0x050000c0, 0x80000080, (volatile u32 *)0x040000d4);
    } else {
        s32 scale = 0x10000 - level * 1092;
        screen->brightness = scale;
        Graphics_ScaleRgb555Clamped(palette, (u16 *)0x050000c0, scale, 128);
    }
    *(volatile u16 *)0x04000208 = ime;
}

