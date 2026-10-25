// Draft, differ 95 of 59: scales the 224 colours of the palette copy at the
// start of heap block 32 into the work area 0xe00 bytes on, one halfword per
// channel: blue unscaled, green and red multiplied by scale; each channel is
// kept above its 10 low bits and capped at 0x7c00. Left: the native keeps the
// scale in ip, builds 31 and 0x7c00 inside the loop and holds the loop bound in
// lr and the mask in r8; the alignment is poor.
#include "types.h"
#include "ram_buffer.h"

void Func_02000580(s32 scale) {
  u16 *source = Ram_HeapSlots->block32;
  u16 *channel = (u16 *)((u8 *)source + 0xe00);
  u32 i, value, color;

  for (i = 0; i < 224; i++) {
    color = *source++;
    value = ((color >> 10) & 31) << 10;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    value = (scale * ((color >> 5) & 31)) & ~0x3ff;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    value = (scale * (color & 31)) & ~0x3ff;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
  }
}
