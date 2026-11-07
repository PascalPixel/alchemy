#include "types.h"
#include "ram_buffer.h"
#include "system/io_reg.h"
#include "system/dma.h"

extern s8 gData_02004694;
extern s8 gData_02004695;
extern s16 gData_020046a0[];
s32 Math_Sine(s32 angle);

void Func_020024b0(void) {
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  s16 *line = gData_020046a0;
  s32 i;
  volatile u16 *channel;

  REG_BLDALPHA = gData_02004694 | 0x1000;
  if ((*Ram_FrameCount & 15) == 0) {
    gData_02004694 += gData_02004695;
    if (gData_02004694 == 8 || gData_02004694 == 4)
      gData_02004695 *= -1;
  }
  for (i = 0; i != 160; i++) {
    *line =
        (scroll[0] >> 16) +
        (Math_Sine((i + (u8)*Ram_FrameCount + (scroll[1] >> 16)) << 9) >> 13);
    line++;
  }
  line = gData_020046a0;
  REG_BG1HOFS = line[0];
  channel = REG_DMA0;
  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  channel[5];
  Dma_Set(line + 1, (void *)&REG_BG1HOFS, 0xa2600001, (volatile u32 *)channel);
}
