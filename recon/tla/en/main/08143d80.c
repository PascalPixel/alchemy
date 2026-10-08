#include "types.h"
#include "iwram_call.h"
#include "system.h"

extern u8 BtlWk[];

typedef s32 (*ClearFn)(void *dst, s32 size);
typedef s32 (*FillFn)(void *dst, s32 size, u32 value);

void BattleFx_SetupCanvasTileMap(void)
{
    u8 **cells = (u8 **)BtlWk;
    void *canvas = cells[31];
    struct BattleScreen *screen = (struct BattleScreen *)cells[0];
    struct BattleLayer *layer = (struct BattleLayer *)cells[35];
    s32 row;
    s32 col;
    s32 off;

    BattleFx_ClearActorTimers();
    BattlePresentation_ConfigurePaletteFadeFar(2, screen->palette_fade, 0);
    gBgScroll[3] = 32;
    layer->offset = 0;
    ClearWords(Iwram_ClearWords, (void *)0x06003fc0, 64);
    FillWords(Iwram_FillWords, (void *)0x0600f900, 0x200, -1);
    off = 0;
    for (row = 0; row != 16; row++) {
        for (col = 0; col != 32; col++, off += 2) {
            if (col > 15)
                *(volatile u16 *)(0x0600fb00 + off) = 0xff;
            else
                *(volatile u16 *)(0x0600fb00 + off) = row * 16 + 0x100 + col;
        }
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
    ClearWords(Iwram_ClearWords, canvas, 0x4000);
    ClearWords(Iwram_ClearWords, (void *)0x06004000, 0x4000);
    WaitFrames(1);
}
