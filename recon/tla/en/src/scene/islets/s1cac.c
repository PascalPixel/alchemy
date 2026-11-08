#include "../../../../../../games/tla/src/scene/islets/islets.h"
#include "dma.h"
#include "io_reg.h"

extern u8 gData_02004f04;
extern s16 gData_02004f10[2][160];

/// Applies BG1 horizontal line scrolling with HBlank DMA.
void Func_02001cac(void) {
  s16 *lines = gData_02004f10[gData_02004f04];
  s32 first = lines[0];
  volatile u16 *scroll;
  volatile u16 *dma;

  /* FAKEMATCH: keeps the first line's load before the register address is built
   */
  __asm__("" : : "r"(first));
  scroll = &REG_BG1HOFS;
  *scroll = first;
  dma = REG_DMA0;

  dma[5] &= SPLIT_CONSTANT(0xc500, 255); // Clear start timing and repeat.
  dma[5] &= SPLIT_CONSTANT(0x7f00, 255); // Stop.
  lines++;
  (void)dma[5];
  Dma_Set(lines, (void *)scroll, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)dma);
}
