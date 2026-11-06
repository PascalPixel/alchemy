// Matches: differ 0 of 59 (tla-en, 2026-11-06).
// Scales the 224 colours of the palette copy at the start of heap block 32 into
// the work area 0xe00 bytes on, one halfword per channel: blue unscaled, green
// and red multiplied by scale; each channel is kept above its 10 low bits and
// capped at 0x7c00.
// What settled the allocation: mask set right after the colour is read and
// applied to all three channels (blue's emits nothing, but without it mask
// ranks below i: differ 38), and each store written *channel++, which keeps
// color live long enough to rank below the 0x7c00 cap so color takes r3.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"

void Func_02000580(s32 scale) {
  u16 *source = Ram_HeapSlots->block32;
  u16 *channel = (u16 *)((u8 *)source + 0xe00);
  u32 count = 224;
  u32 i = 0;
  u32 color, value, mask;

  do {
    color = *source++;
    mask = ~0x3ff;
    value = (color >> 10) & 31;
    value <<= 10;
    // Clears nothing, but the native matches only with it.
    value &= mask;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    value = (color >> 5) & 31;
    value *= scale;
    value &= mask;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    value = color & 31;
    value *= scale;
    value &= mask;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    i++;
  } while (i < count);
}
