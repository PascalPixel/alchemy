/* Near miss, differ 10 of 114: The Broken Seal's BattleFx_SetupCanvasTileMap with
   ⚓️'s slots (the background view is the 0xb0 slot) and its IWRAM calls through
   forwarders (differ 28 called directly). Left: three places where the native
   loads a pool constant ahead of a constant it builds: the loop preheader
   (ldr r5, =0x0600fb00; movs r2, #128; ldr r7, =0xff; lsls r2, r2, #1), the
   window sizes (ldr r1, =0xf0; movs r3, #128; ldr r2, =0x1088; lsls r3, r3, #19)
   and WININ (ldr r3, =0x3537 before movs r2, #128). Pinning WININ's value behind a
   barrier that holds r2, as f18284c.c does, orders that row but reorders the
   literal pool (differ 18). */
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_work.h"
#include "battle_display.h"
#include "bg_scroll.h"

void Func_081434d8(void);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 background, s32 value);

static __inline__ s32 ClearWords_(void *destination, s32 size)
{
    return Iwram_ClearWords(destination, size);
}

static __inline__ s32 FillWords_(void *destination, s32 size, u32 value)
{
    return Iwram_FillWords(destination, size, value);
}

void BattleFx_SetupCanvasTileMap(void)
{
    void *canvas = Ram_HeapSlots->battle_fx_work[1];
    struct BattleWork *screen = Ram_HeapSlots->battle_work;
    struct BattleBackgroundView *layer = (struct BattleBackgroundView *)Ram_HeapSlots->transition_work;
    s32 row;
    s32 col;
    s32 off;

    Func_081434d8();
    BattlePresentation_ConfigurePaletteFadeFar(2, *(u16 *)screen->unknown_670, 0);
    gBgScroll[1].y = 32;
    layer->second_mode = 0;
    ClearWords_((void *)0x06003fc0, 64);
    FillWords_((void *)0x0600f900, 0x200, -1);
    off = 0;
    for (row = 0; row != 16; row++)
    {
        for (col = 0; col != 32; col++, off += 2)
        {
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
    ClearWords_(canvas, 0x4000);
    ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
    WaitFrames(1);
}
