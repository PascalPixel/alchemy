// Twin: The Broken Seal's BattleEffect_SetupCanvasTileMap, with The Lost Age's
// heap slots (the background view is the 0xb0 slot). The IWRAM calls go
// through forwarders; called directly, 16 of 114 instructions differ in tla-en.
#include "types.h"
#include "system.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "battle_work.h"
#include "battle_display.h"
#include "bg_scroll.h"

void BattleEffect_ClearActorTimers(void);
void BattleStaging_ConfigurePaletteFadeFar(s32 mode, s32 background,
                                                s32 value);

static __inline__ s32 ClearWords_(void *destination, s32 size) {
  return Iwram_ClearWords(destination, size);
}

static __inline__ s32 FillWords_(void *destination, s32 size, u32 value) {
  return Iwram_FillWords(destination, size, value);
}

void Func_08143d80(void) {
  void *canvas = Ram_HeapSlots->battleFxWork[1];
  struct BattleWork *screen = Ram_HeapSlots->battleWork;
  struct BattleBackgroundView *layer =
      (struct BattleBackgroundView *)Ram_HeapSlots->transitionWork;
  s32 row;
  s32 col;
  s32 off;

  BattleEffect_ClearActorTimers();
  BattleStaging_ConfigurePaletteFadeFar(2, *(u16 *)screen->unk670, 0);
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
  REG_WIN0H = 0xf0;
  REG_WIN0V = 0x1088;
  REG_WIN1H = 0xf0;
  REG_WIN1V = 0x1088;
  REG_WININ = 0x3537;
  REG_WINOUT = 0x3f21;
  REG_BLDALPHA = 0x100e;
  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  wait(1);
}
