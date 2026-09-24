/* 2026-09-24: hand-written, 22 differing halfwords, same size apart from the
   trailing alignment. Mid-function pools and the tile-map loop match; the
   IWRAM clear/fill calls load their destination before the routine, and the
   map base, row counter and row base take r4/r5/r6 instead of r5/r6/r4. */

#include "TYPES.H"
#include "SYSTEM.H"

typedef void (*ClearFn)(void *dst, s32 size);
typedef void (*FillFn)(void *dst, s32 size, s32 value);

static __inline__ void ClearWords(ClearFn clear, void *dst, s32 size)
{
    clear(dst, size);
}

static __inline__ void FillWords(FillFn fill, void *dst, s32 size, s32 value)
{
    fill(dst, size, value);
}

struct BattleScreen {
    u8 unknown_000[0x648];
    u16 palette_fade;
};

struct BattleLayer {
    u8 unknown_00[12];
    s32 offset;
};

extern u16 Data_03001ad0[];
void Runtime_ApplyValueToWork7818(void);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 fade, s32 arg);

void Func_080cdd58(void)
{
    u8 **cells = (u8 **)0x03001e74;
    void *canvas = cells[31];
    struct BattleScreen *screen = (struct BattleScreen *)cells[0];
    struct BattleLayer *layer = (struct BattleLayer *)cells[35];
    u16 *map;
    s32 row;
    s32 col;
    s32 off;
    s32 base;

    Runtime_ApplyValueToWork7818();
    BattlePresentation_ConfigurePaletteFadeFar(2, screen->palette_fade, 0);
    Data_03001ad0[3] = 32;
    layer->offset = 0;
    ClearWords((ClearFn)0x03000164, (void *)0x06003fc0, 64);
    FillWords((FillFn)0x03000168, (void *)0x0600f900, 0x200, -1);
    off = 0;
    base = 0;
    for (row = 0; row != 16; row++) {
        s32 tile = base + 0x100;
        for (col = 0; col != 32; col++) {
            if (col > 15)
                *(volatile u16 *)(0x0600fb00 + off) = 0xff;
            else
                *(volatile u16 *)(0x0600fb00 + off) = tile;
            tile++;
            off += 2;
        }
        base += 16;
    }
    *(volatile u16 *)0x04000000 = 0x7741;
    *(volatile u16 *)0x0400000a = 0x1f81;
    *(volatile u16 *)0x04000050 = 0x3f42;
    *(volatile u16 *)0x04000040 = 0xf0;
    *(volatile u16 *)0x04000044 = 0x1088;
    *(volatile u16 *)0x04000042 = 0xf0;
    *(volatile u16 *)0x04000046 = 0x1088;
    *(volatile u16 *)0x04000048 = 0x3537;
    *(volatile u16 *)0x0400004a = 0x3f21;
    *(volatile u16 *)0x04000052 = 0x100e;
    ClearWords((ClearFn)0x03000164, canvas, 0x4000);
    ClearWords((ClearFn)0x03000164, (void *)0x06004000, 0x4000);
    WaitFrames(1);
}

