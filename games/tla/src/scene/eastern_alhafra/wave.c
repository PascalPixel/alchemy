#include "types.h"
#include "io_reg.h"
#include "dma.h"
#include "ram_buffer.h"
#include "map.h"
#include "event_inline.h"

extern u8 gData_020054f0[];
extern u16 gData_020056e0[2][160];
extern u32 gFrameCount;

// BG1's wave: each frame the finished table of scanline offsets is handed to
// an HBlank DMA, while the other table is filled for the next frame.

/// Starts DMA 0 copying the finished table into BG1's horizontal offset at
/// each HBlank.
// With ☀️'s way of stopping DMA 0.
void Func_02000a44(void) {
  u16 *line = gData_020056e0[gData_020054f0[0] ^ 1];
  volatile u16 *channel;

  REG_BG1VOFS = line[0];
  channel = REG_DMA0;
  channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  channel[5] &= ~DMA_ENABLE;
  line++;
  (void)channel[5];
  Dma_Set(line, (void *)&REG_BG1VOFS, DMA_HBLANK_HALFWORD_PER_LINE,
          (volatile u32 *)channel);
}

/// Fills the other table: each of the 160 lines takes layer 2's scroll plus a
/// sine of the frame count less the line.
void Func_02000a98(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[2];
  u16 *line;
  s32 i = 0;

  line = gData_020056e0[gData_020054f0[0] ^ 1];
  for (i = 0; i != 160; i++) {
    *line++ =
        (layer->y >> 16) +
        (Math_Sine(((u8)gFrameCount - i - (layer->y >> 16) + 160) << 9) >> 15);
  }
  gData_020054f0[0] ^= 1;
}

void Func_02000af8(void) {
  Task_AddFrame(Func_02000a98, 0x480);
  Task_AddFrame(Func_02000a44, 0xc80);
}
