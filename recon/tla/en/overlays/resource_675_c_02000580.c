// Draft, differ 41 of 59 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Measured under -Os, 5 November 2026: differ 41 (tla-en); notes below may predate it.
// Draft, differ 57 of 59, skeleton gap 15/6 (tla-en, 2026-11-01)
// Scales the 224 colours of the palette copy at the start of heap block 32 into
// the work area 0xe00 bytes on, one halfword per channel: blue unscaled, green
// and red multiplied by scale; each channel is kept above its 10 low bits and
// capped at 0x7c00.
// Left: allocation only; the control flow, loads and constants all line up, and
// the skeleton gap is the moves below. The listing has i in r7, scale in ip and
// mask in r8, with r6 as reload scratch (mov r6, ip; muls r6, r2; mov r6, r8);
// here scale is r7, i is ip and mask is r6. The cause is global.c's order:
// scale (10/36) ranks above i (14/64), because cse's REG_EQUAL on i = 0 makes
// update_equiv_regs double i's live length (32 to 64) before no_equiv drops the
// note. The listing needs mask > i > scale. Undoubling i by giving it an earlier
// non-constant value (diagnostic, not kept: a reused local is a disguise) does
// produce the listing's muls and r8 reloads, but puts i above mask (differ 65).
// Kept: the goto loop; a for, do or while(1) hoists 31, 0x7c00 and the mask out
// of the loop (differ 95). Each step is its own statement: a combined expression
// makes a block-local pseudo that takes r3 from color (differ 83). mask stays a
// local: inline it and its load falls after the first compare (differ 59).
// Tried without gain: goto into a bottom test, count before or after i, i++ and
// source++ placement, multiply operand order, locals for 31 and 0x7c00 (same
// differ, so left out), inner-scope locals.
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
    color = *source;
    value = (color >> 10) & 31;
    mask = ~0x3ff;
    value <<= 10;
    source++;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel = value;
    value = (color >> 5) & 31;
    value *= scale;
    value &= mask;
    channel++;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel = value;
    value = color & 31;
    value *= scale;
    value &= mask;
    channel++;
    if (value > 0x7c00)
      value = 0x7c00;
    i++;
    *channel = value;
    channel++;
  } while (i < count);
}
