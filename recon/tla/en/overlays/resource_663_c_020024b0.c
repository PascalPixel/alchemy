// Draft, differ 87 of 84: a water shimmer: sets the blend from a weight that
// swings between 4 and 8 every 16 frames, fills a 160-line table of
// horizontal offsets from a sine wave over the map's scroll, and starts the
// table's HBlank DMA into BG1's horizontal offset. Left: the native's line
// loop reloads the frame count and the scroll each pass (no loop
// optimization), and the weight's update is ordered otherwise.
#include "types.h"
#include "ram_buffer.h"
#include "system/io_reg.h"
#include "system/dma.h"

extern u32 gFrameCount;
extern s8 gData_02004694[2];
extern u16 gData_020046a0[];
s32 Math_Sine(s32 angle);

void Func_020024b0(void) {
  s16 *scroll = (s16 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  u16 *line = gData_020046a0;
  s8 weight, step;
  s32 i;

  REG_BLDALPHA = gData_02004694[0] | 0x1000;
  if ((gFrameCount & 15) == 0) {
    step = gData_02004694[1];
    weight = gData_02004694[0] + step;
    gData_02004694[0] = weight;
    if (weight == 8 || weight == 4)
      gData_02004694[1] = -step;
  }
  i = 0;
wave:
  *line = scroll[1] + (Math_Sine((i + (u8)gFrameCount + scroll[3]) << 9) >> 13);
  i++;
  line++;
  if (i != 160)
    goto wave;
  REG_BG1HOFS = gData_020046a0[0];
  ((volatile u16 *)0x040000b0)[5] &= 0xc5ff;
  ((volatile u16 *)0x040000b0)[5] &= 0x7fff;
  ((volatile u16 *)0x040000b0)[5];
  Dma_Set(&gData_020046a0[1], (void *)&REG_BG1HOFS, 0xa2600001,
          (volatile u32 *)0x040000b0);
}
