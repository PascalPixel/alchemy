// Draft, differ 96 of 135: a heat haze: fills a 160-line table of three
// halfwords a line (BG0's two offsets and BG1's horizontal offset) from a
// sine wave narrowing down the screen with random jitter, keeps the blend
// weight swinging between 4 and 8 every 16 frames, and starts the table's
// HBlank DMA into the background offsets. Func_020024b0 is its simpler
// twin. Left: as Func_020024b0's draft, the native reaches the weight's second byte through its own literal and reloads the frame count and scroll each line; the jitter's clamp shares one store.
#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system/io_reg.h"
#include "system/dma.h"

extern u32 gFrameCount;
extern s8 gData_02004696[2];
extern u16 gData_020046a0[];
s32 Math_Sine(s32 angle);
u16 Random16Far(void);

void Func_0200257c(void) {
  s16 *scroll = (s16 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  u16 *line = gData_020046a0;
  s32 i, wave, spread;
  s8 weight, step;

  for (i = 0; i != 160; i++) {
    wave = Math_Sine((i + (u8)gFrameCount) << 8);
    spread = (160 - i) >> 2;
    line[0] = (Iwram_MulQ16(wave, (144 - i) << 10) >> 11) - 4;
    line[0] += (u32)Random16Far() % (u32)(spread * 2) - spread;
    if ((s16)line[0] > 56)
      line[0] -= 56;
    else if ((s16)line[0] < -64)
      line[0] += 64;
    line[1] = 0;
    line[2] = scroll[1] + ((wave + scroll[3]) >> 13);
    line += 3;
  }
  REG_BLDALPHA = gData_02004696[0] | 0x1000;
  if ((gFrameCount & 15) == 0) {
    step = gData_02004696[1];
    weight = gData_02004696[0] + step;
    gData_02004696[0] = weight;
    if (weight == 8 || weight == 4)
      gData_02004696[1] = -step;
  }
  *(volatile u16 *)REG_BG0HOFS = gData_020046a0[0];
  REG_BG1HOFS = gData_020046a0[2];
  ((volatile u16 *)0x040000b0)[5] &= 0xc5ff;
  ((volatile u16 *)0x040000b0)[5] &= 0x7fff;
  ((volatile u16 *)0x040000b0)[5];
  Dma_Set(&gData_020046a0[3], REG_BG0HOFS, 0xa2600003,
          (volatile u32 *)0x040000b0);
}
