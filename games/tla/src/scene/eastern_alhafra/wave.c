#include "types.h"
#include "call.h"
#include "io_reg.h"
#include "dma.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gData_020054f0[];
extern u16 gData_020056e0[2][160];
extern u32 gFrameCount;

s32 Math_Sine(s32 angle);
void Scheduler_AddOrUpdateCallback();

// BG1's wave: each frame the finished table of scanline offsets is handed to
// an HBlank DMA, while the other table is filled for the next frame.

/// Starts DMA 0 copying the finished table into BG1's horizontal offset at
/// each HBlank.
// With ☀️'s DMA 0 stop sequence.
void Func_02000a44(void) {
  u16 *line = gData_020056e0[gData_020054f0[0] ^ 1];
  volatile u16 *channel;

  *(volatile u16 *)0x04000016 = line[0];
  /* FAKEMATCH: the one-pass block takes the channel address before the masks
   * are built (alchemy permute). */
  do {
    channel = REG_DMA0;
  } while (0);
  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  line++;
  /* FAKEMATCH: steps the cursor before the stop's dummy read; without the
   * barrier it comes after (differ 2). */
  __asm__ volatile("" : "+r"(line));
  (void)channel[5];
  Dma_Set(line, (void *)0x04000016, 0xa2600001, (volatile u32 *)channel);
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
  Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02000a98, 0x480);
  Value2((s32 (*)())Scheduler_AddOrUpdateCallback, (s32)Func_02000a44, 0xc80);
}
