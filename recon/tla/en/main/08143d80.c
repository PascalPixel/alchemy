// Near miss, differ 6 of 114 (10 as plain C): The Broken Seal's
// BattleFx_SetupCanvasTileMap with ⚓️'s slots (the background view is the
// 0xb0 slot) and its IWRAM calls through forwarders (differ 28 called
// directly). Left: two places where the native issues a pool load between the
// halves of a constant it builds, which agscc's sched2 ranks one below the
// shift, as if the native's loads cost a cycle more: the loop preheader (ldr
// r5, =0x0600fb00; movs r2, #128; ldr r7, =0xff; lsls r2, r2, #1) and the
// window height (movs r3, #128; ldr r2, =0x1088; lsls r3, r3, #19). The
// window width and WININ's value load first through pins in one-pass blocks,
// which add no bytes to the pool estimate (a pin behind a barrier asm, as
// f18284c.c does, reorders the literal pool: differ 18). Pinning the map base
// in r5 the same way rewrites the loop's registers and addressing.
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_work.h"
#include "battle_display.h"
#include "bg_scroll.h"

void BattleFx_ClearActorTimers(void);
void BattlePresentation_ConfigurePaletteFadeFar(s32 mode, s32 background,
                                                s32 value);

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

static __inline__ s32 FillWords_(void *destination, s32 size, u32 value) {
  return Iwram_FillWords(destination, size, value);
}

void BattleFx_SetupCanvasTileMap(void) {
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  struct BattleBackgroundView *layer =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 off;

  BattleFx_ClearActorTimers();
  BattlePresentation_ConfigurePaletteFadeFar(2, *(u16 *)screen->unk670, 0);
  gBgScroll[1].y = 32;
  layer->secondMode = 0;
  ClearWords_((void *)0x06003fc0, 64);
  FillWords_((void *)0x0600f900, 0x200, -1);
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
  {
    /* FAKEMATCH(fixed register): the window width loaded into r1 inside the
     * block below, before the window address starts; with neither this nor
     * WININ's pin, 10 of 114 instructions differ in tla-en. */
    register u16 width asm("r1");

    /* FAKEMATCH(one-pass block): ends the scheduling region after the load,
     * with no bytes for the pool's estimate. */
    do {
      width = 0xf0;
    } while (0);
    REG_WIN0H = width;
    REG_WIN0V = 0x1088;
    REG_WIN1H = width;
    REG_WIN1V = 0x1088;
  }
  {
    /* FAKEMATCH(fixed register): WININ's value loaded into r3 before its
     * address starts, as above. */
    register u16 inside asm("r3");

    /* FAKEMATCH(one-pass block): as above. */
    do {
      inside = 0x3537;
    } while (0);
    REG_WININ = inside;
  }
  REG_WINOUT = 0x3f21;
  REG_BLDALPHA = 0x100e;
  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  WaitFrames(1);
}
