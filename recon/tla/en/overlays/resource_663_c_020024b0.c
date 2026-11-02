// Matches: differ 0 (tla-en, 2026-11-01)
// A water shimmer: sets the blend from a weight that swings between 4 and 8
// every 16 frames, fills a 160-line table of horizontal offsets from a sine
// wave over the map's scroll, and starts the table's HBlank DMA into BG1's
// horizontal offset. Func_0200257c is its heat-haze twin.
// No devices. Proved, keep:
// - The frame count is Ram_FrameCount at both sites: as the gFrameCount symbol
//   its pool load is hoisted out of the loop into r8 (differ 17). The fixed
//   address is not, and that also frees the loop from goto.
// - scroll is Q16, read as `>> 16`; a halfword view gives ldrh on scroll[0].
// - `line` is rewound to the table's start after the loop: a separate `table`,
//   or naming gData_020046a0 directly, swaps r6 and r7 (differ 26). Check this
//   against S2 on adoption: it is the same table walker, not a second value.
// - `*= -1` keeps the sign extension before negs (`= -x`, differ 8).
// - The second byte needs its own literal, so it is its own symbol (one s8[2],
//   differ 19): the listing's gData_02004694 + 1 has to be split in the data
//   on adoption, as in the twin.
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
