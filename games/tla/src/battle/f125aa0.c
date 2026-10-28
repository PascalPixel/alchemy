#include "fixed_math.h"
#include "types.h"
#include "system.h"
#include "io_reg.h"

void Fade_ToBlack(s32 arg0);

s32 Func_08125aa0(void) {
  /* FAKEMATCH(volatile): the four saved BG control words volatile, so each
   * keeps its stack slot; as plain u16 locals, 65 of 94 instructions differ
   * in tla-en. */
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
  *reg = bg1cnt | 0x40;
  bg2cnt = *++reg;
  *reg = bg2cnt | 0x40;
  bg3cnt = *++reg;
  *reg = bg3cnt | 0x40;
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
