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
  REG_DISPCNT = DISPCNT_WIN1_ON | DISPCNT_WIN0_ON | DISPCNT_OBJ_ON |
                DISPCNT_BG2_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                DISPCNT_OBJ_1D_MAP | DISPCNT_MODE_1;
  REG_BG1CNT = BGCNT_SCREEN_BASE(31) | BGCNT_256_COLORS | BGCNT_PRIORITY(1);
  REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
  REG_WIN0H = WIN_RANGE(0, 240);
  REG_WIN0V = WIN_RANGE(16, 136);
  REG_WIN1H = WIN_RANGE(0, 240);
  REG_WIN1V = WIN_RANGE(16, 136);
  REG_WININ = WININ_WIN1_CLR | WININ_WIN1_OBJ | WININ_WIN1_BG2 |
              WININ_WIN1_BG0 | WININ_WIN0_CLR | WININ_WIN0_OBJ |
              WININ_WIN0_BG2 | WININ_WIN0_BG1 | WININ_WIN0_BG0;
  REG_WINOUT = WINOUT_WINOBJ_ALL | WINOUT_WIN01_CLR | WINOUT_WIN01_BG0;
  REG_BLDALPHA = BLEND_ALPHA(14, 16);
  ClearWords_(canvas, 0x4000);
  ClearWords_(BG_CHAR_BLOCK(1), 0x4000);
  wait(1);
}
