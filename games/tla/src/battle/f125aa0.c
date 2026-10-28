#include "fixed_math.h"
#include "types.h"
#include "system.h"

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

  bg0cnt = *(volatile u16 *)0x04000008;
  *(volatile u16 *)0x04000008 = bg0cnt | 0x40;
  reg = (volatile u16 *)0x04000008;
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
    *(volatile u16 *)0x0400004c = (i << 8) | i;
    wait(1);
  }

  *(volatile u16 *)0x04000000 = 1;
  wait(4);
  saved = bg0cnt;
  reg = (volatile u16 *)0x04000008;
  *reg++ = saved;
  *reg++ = bg1cnt;
  *reg++ = bg2cnt;
  *reg = bg3cnt;
  return 0;
}
