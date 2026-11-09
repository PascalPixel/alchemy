#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system/io_reg.h"
#include "system/dma.h"
#include "event_inline.h"

extern u32 gFrameCount;
extern s8 gData_02004696;
extern s8 gData_02004697;
extern s16 gData_020046a0[];
u32 Random16Far(void);

void Func_0200257c(void) {
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  s16 *line = gData_020046a0;
  s32 i, wave, spread, value;
  s16 *table;
  volatile u16 *channel;

  for (i = 0; i != 160; i++) {
    spread = (160 - i) >> 2;
    wave = Math_Sin((i + (u8)gFrameCount) << 8);
    /* FAKEMATCH(temporary): value holds line[0]'s first value, then
     * wave + y for line[2]; as the plain statements, 41 of 135 instructions
     * differ in tla-en. */
    value = (Iwram_MulQ16(wave, (144 - i) << 10) >> 11) - 4;
    line[0] = value;
    line[0] += Random16Far() % (spread * 2) - spread;
    if (line[0] > 56)
      line[0] -= 56;
    else if (line[0] < -64)
      line[0] += 64;
    line[1] = 0;
    value = wave + (scroll[1] >> 16);
    line[2] = (scroll[0] >> 16) + (value >> 13);
    line += 3;
  }
  REG_BLDALPHA = gData_02004696 | 0x1000;
  if ((gFrameCount & 15) == 0) {
    gData_02004696 += gData_02004697;
    if (gData_02004696 == 8 || gData_02004696 == 4)
      gData_02004697 *= -1;
  }
  table = gData_020046a0;
  *(volatile u16 *)REG_BG0HOFS = table[0];
  table += 2;
  REG_BG1HOFS = table[0];
  channel = REG_DMA0;
  channel[5] &= 0xc5ff;
  channel[5] &= 0x7fff;
  channel[5];
  Dma_Set(table + 1, REG_BG0HOFS, 0xa2600003, (volatile u32 *)channel);
}
