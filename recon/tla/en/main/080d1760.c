// Matches (tla-it, 2026-11-07), with one tagged dead device (work stepped
// and stepped back) found by the permuter; nothing untagged closed the copy.
// Japanese, German, Spanish, French and Italian keep this as a function of
// its own: it packs the target channels back into the source palette as
// RGB555. English has no such function; its one packing loop is inline in
// BattlePalette_UpdateBlend (games/tla/src/field/field_sequence/f0d0ca0.c),
// reading current into a packed page. Without the device the native's copy
// of the work pointer (r3 to r0, target addressed from r0) is missing.
#include "types.h"
#include "palette_buffers.h"
#include "ram_buffer.h"

void BattleEffect_PackTargetBuffer(void) {
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
  /* FAKEMATCH(dead): work stepped and stepped back after the loop, which
   * keeps it live apart from packed; as without the two updates, 7 of 34
   * instructions differ in tla-it. */
  work++;
  work--;
}
