// Matches: differ 0 of 135 (tla-en, 2026-11-06).
// Under -Os, 5 November: differ 35 of 135 (tla-en); older figures below predate it.
// A heat haze: fills a 160-line table of three halfwords a line (BG0's two
// offsets and BG1's horizontal offset) from a sine wave narrowing down the
// screen with random jitter, keeps the blend weight swinging between 4 and 8
// every 16 frames, and starts the table's HBlank DMA into the background
// offsets. Func_020024b0 is its simpler twin.
// 6 November: one local, value, set twice (line[0]'s first value, then
// wave + y for line[2]) matches with no other device, and replaces the
// once-only block around REG_BLDALPHA the 5 November draft kept (33 of 135).
// The plain form is 35 of 135.
// Measured with the sum in a local set once, y first in every spelling:
// differ 39; `offset = wave + y; offset = x + (offset >> 13); line[2] = offset;`
// differ 10. No plain form found: a dead `= 0` default, `phase = wave;
// phase += y`, `wave += y`, an inner-block local, declaration order, s16 and
// u16 views of scroll's halves and casts on either term all stay between 28
// and 48.
// Proved, keep: scroll is Q16, read as `>> 16` (a halfword view narrows the
// add to ldrh); the table is s16; `*= -1` keeps the sign extension before
// negs; the walked table pointer gives `adds r0, #4` and `adds r0, #2` (a
// set-once pointer, or none, differ 44). The second byte needs its own
// literal, so it is its own symbol: the listing's gData_02004696 + 1 has to be
// split in the data on adoption.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system/io_reg.h"
#include "system/dma.h"

extern u32 gFrameCount;
extern s8 gData_02004696;
extern s8 gData_02004697;
extern s16 gData_020046a0[];
s32 Math_Sine(s32 angle);
u32 Random16Far(void);

void Func_0200257c(void) {
  s32 *scroll = (s32 *)((u8 *)Ram_HeapSlots->mapWork + 0x178);
  s16 *line = gData_020046a0;
  s32 i, wave, spread, value;
  s16 *table;
  volatile u16 *channel;

  for (i = 0; i != 160; i++) {
    spread = (160 - i) >> 2;
    wave = Math_Sine((i + (u8)gFrameCount) << 8);
    /* FAKEMATCH(temporary): value holds line[0]'s first value, then
     * wave + y for line[2]; as the plain statements, 35 of 135 instructions
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
