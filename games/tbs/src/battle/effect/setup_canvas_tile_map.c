#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "iwram_call.h"
#include "system.h"
#include "io_reg.h"

#define BattlePresentation_ConfigurePaletteFadeFar ((void (*)(s32,s32,s32))BattlePresentation_ConfigurePaletteFadeFar)

extern u8 BtlWk[];

typedef s32 (*ClearFn)(void *dst, s32 size);
typedef s32 (*FillFn)(void *dst, s32 size, u32 value);

static __inline__ void ClearWords(ClearFn clear, void *dst, s32 size)
{
    /* FAKEMATCH: needed to match: direct calls share the 0x4000 size in r5 and move the clear routine to r6 */
    clear(dst, size);
}

static __inline__ void FillWords(FillFn fill, void *dst, s32 size, s32 value)
{
    /* FAKEMATCH: needed to match: a direct call loads the destination before the routine and reverses their pool words */
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

extern u16 gBgScroll[];

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
    REG_DISPCNT = 0x7741;
    REG_BG1CNT = 0x1f81;
    REG_BLDCNT = 0x3f42;
    REG_WIN0H = 0xf0;
    REG_WIN0V = 0x1088;
    REG_WIN1H = 0xf0;
    REG_WIN1V = 0x1088;
    REG_WININ = 0x3537;
    REG_WINOUT = 0x3f21;
    REG_BLDALPHA = 0x100e;
    ClearWords(Iwram_ClearWords, canvas, 0x4000);
    ClearWords(Iwram_ClearWords, BG_CHAR_BLOCK(1), 0x4000);
    WaitFrames(1);
}
