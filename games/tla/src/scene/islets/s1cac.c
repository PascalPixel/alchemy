#include "islets.h"
#include "dma.h"
#include "io_reg.h"

extern u8 gData_02004f04;
extern s16 gData_02004f10[2][160];

// 走査線ごとの BG1 横スクロールを HBlank DMA に掛ける
void Func_02001cac(void) {
  s16 *lines = gData_02004f10[gData_02004f04];
  s32 first = lines[0];
  volatile u16 *scroll;
  volatile u16 *dma;

  /* FAKEMATCH: keeps the first line's load before the register address is built
   */
  __asm__("" : : "r"(first));
  scroll = (volatile u16 *)0x04000014;
  *scroll = first;
  dma = REG_DMA0;

  /* FAKEMATCH: the one-pass block reads the control before its mask is started;
   * without it the mask is started first (differ 2) */
  do {
    dma[5] &= 0xc5ff; // 開始タイミングとリピートを消す
    dma[5] &= 0x7fff; // 止める
    lines++;
    /* FAKEMATCH: keeps the cursor step before the last control read */
    __asm__("" : : "r"(lines));
  } while (0);
  (void)dma[5];
  Dma_Set(lines, (void *)scroll, 0xa2600001, (volatile u32 *)dma);
}
