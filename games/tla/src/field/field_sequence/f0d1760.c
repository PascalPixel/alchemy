/// @file
/// Packs the effect's target channels into its palette.

#include "types.h"
#include "palette_buffers.h"
#include "ram_buffer.h"

/// Packs the 448 target colours, three channels each, back into the effect's
/// palette as RGB555.
void Func_080d1760(void) {
  struct BattleEffectBuffers *work = Ram_HeapSlots->block32;
  u16 *packed = work->palette;
  s32 blue = 0x7c00;
  u16 *current;
  s32 i;

  i = 0x1c0;
  current = work->target;
  for (; i != 0; i--) {
    *packed++ = (current[0] & blue) | ((current[1] >> 5) & 0x3e0) |
                ((current[2] >> 10) & 0x1f);
    current += 3;
  }
  /* FAKEMATCH(dead): two updates of work that cancel, after the loop, keep it
   * live apart from packed; without them, 7 of 34 instructions differ in
   * tla-en. */
  work++;
  work--;
}
