#include "fixed_math.h"
#include "types.h"
#include "system.h"
#include "io_reg.h"

void Fade_ToBlack(s32 arg0);

/// Fades the screen to black over sixteen frames as every background's
/// mosaic grows, then shows no layer. Sets REG_BLDCNT to darken the
/// backgrounds, leaves REG_MOSAIC at its largest and REG_DISPCNT in mode 1
/// with every layer off; the four BGxCNT are put back as they were. It draws
/// four random numbers a frame, which only advances the random number
/// generator. Returns 0.
s32 BattleTransition_RunMosaicFadeOut(void) {
  // volatile: the four saved BGxCNT values kept on the stack, each stored and
  // loaded again as in the ROM; without volatile, 65 of 94 instructions differ
  // in tla-en.
  volatile u16 bg0cnt;
  volatile u16 bg1cnt;
  volatile u16 bg2cnt;
  volatile u16 bg3cnt;
  s32 i;
  volatile u16 *reg;
  u32 saved;

  bg0cnt = REG_BG0CNT;
  REG_BG0CNT = bg0cnt | BGCNT_MOSAIC;
  reg = &REG_BG0CNT;
  bg1cnt = *++reg;
  *reg = bg1cnt | BGCNT_MOSAIC;
  bg2cnt = *++reg;
  *reg = bg2cnt | BGCNT_MOSAIC;
  bg3cnt = *++reg;
  *reg = bg3cnt | BGCNT_MOSAIC;
  reg += 0x21;
  *reg = 0x3eee;

  Fade_ToBlack(16);
  for (i = 0; i <= 15; i++) {
    Random16();
    Random16();
    Random16();
    Random16();
    REG_MOSAIC = (i << 8) | i;
    wait(1);
  }

  REG_DISPCNT = 1;
  wait(4);
  saved = bg0cnt;
  reg = &REG_BG0CNT;
  *reg++ = saved;
  *reg++ = bg1cnt;
  *reg++ = bg2cnt;
  *reg = bg3cnt;
  return 0;
}
