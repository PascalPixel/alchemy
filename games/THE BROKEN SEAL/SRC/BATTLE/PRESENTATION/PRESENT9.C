/* BattlePresentation_SetPaletteLevel: with IME off, copy the battle
   palette to BG palette 6 (level 0) or darken it by level/60 into the
   same place, recording the scale. FAKEMATCH: the IME save sits in a
   one-pass loop around a pointer to its stack slot (taken before the IME
   register address), and the scale is assigned inside the call's argument
   list; the u32 restore temporary loads the saved word before the IME
   address. */
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "DMA.H"
#include "BATTLE_WORK.H"
#include "IO_WRITE_QUEUE.H"

s32 Graphics_ScaleRgb555Clamped(u16 *source, u16 *destination, s32 scale, s32 count);

extern u8 gTransitionWork[];

struct Half {
    u16 v;
};

void BattlePres_UpdateHBlankScroll(void);
void Graphics_BuildSequentialTileTable(void *);
void BattlePresentation_BuildTilemap(void *);

void BattlePresentation_SetPaletteLevel(s32 unused, s32 level)
{
    struct BattleSession *screen = gBattleWork;
    u16 *palette = screen->palette;
    volatile u32 ime;

    /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
    do {
        volatile u32 *slot = &ime;
        u16 *ime_reg = (u16 *)0x04000208;

        /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
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

/* BattlePresentation_ConfigurePaletteFade: start the H-blank scroll
   callback on first use and record the mode; mode 1 also queues a BG2
   control write. Copy the backdrop palette, then either copy the battle
   palette to BG palette 6 or darken each channel by fade into it, and
   rebuild the tile table and tilemap.
   FAKEMATCH: the IME save sits in one-pass loops, as in the IO write
   queue, and the green and blue mask is a one-halfword struct, which keeps
   it a pool constant held across the fade loop as in the ROM. */
void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 value, s32 fade)
{
    s32 *transition = *(s32 **)gTransitionWork;

    if (transition[2] == 0) {
        Scheduler_AddOrUpdateCallback((s32)(BattlePres_UpdateHBlankScroll), 0x4ff);
    }
    transition[2] = mode;

    if (mode == 1) {
        volatile u16 *ime;
        struct IoWriteQueue *q;
        u32 saved;
        s32 count;

        q = &gIoWriteQueue;
        {
            /* FAKEMATCH: removing this one-pass block changes instruction scheduling; see its retained draft. */
            do {
                ime = (volatile u16 *)0x04000208;
                saved = *ime;
            } while (0);
            *ime = (u16)ime;
            count = q->count;
            if (count <= 31) {
                u32 *destination = (u32 *)((u8 *)q + count * 12 + 4);
                *(u16 *)&q->count = count + 1;
                *destination++ = 0x1f83;
                *destination++ = 0x0400000a;
                *destination = 0x20000;
            }
            *ime = saved;
        }
    }

    Dma_Set((void *)0x05000200, (void *)0x050000a0, 0x80000010, (volatile u32 *)0x040000d4);
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;

    if (fade == 0x80) {
        Dma_Set(gBattleWork->palette, (void *)0x050000c0, 0x80000080, (volatile u32 *)0x040000d4);
    } else if (fade != 0) {
        u16 *source = gBattleWork->palette;
        u16 *destination = (u16 *)0x050000c0;
        s32 i;
        struct Half mask;

        mask.v = 0x1f;
        for (i = 0; i != 128; i++) {
            s32 red = source[i] & 31;
            s32 green = (source[i] >> 5) & mask.v;
            s32 blue = (source[i] >> 10) & mask.v;

            if (red > fade) {
                red -= fade;
            } else {
                red = 0;
            }
            if (green > fade) {
                green -= fade;
            } else {
                green = 0;
            }
            if (blue > fade) {
                blue -= fade;
            } else {
                blue = 0;
            }
            destination[i] = (blue << 10) | (green << 5) | red;
        }
    }

    Graphics_BuildSequentialTileTable((void *)0x06003800);
    BattlePresentation_BuildTilemap((void *)0x0600f800);
}
