#include "types.h"

/*
 * resource_37a owner at 0x02000030, 8 bytes: a constant getter --
 * `ldr r0, [pc, #0] / bx lr` over its own one-word pool, returning the
 * in-image address 0x0200aafc (overlay offset 0x2afc).
 *
 * Complete owner: NO PROLOGUE. Saves nothing, no stack, lr never written,
 * bare `bx lr` at 0x02000032. The span is EIGHT bytes and not four:
 * the pool word sits PAST the `bx lr`, at 0x02000034, and belongs to
 * this owner. Measured to the return and then the pool, NOT inferred from
 * the shape -- the stub at 0x02000038 four bytes away has the same
 * cohort membership and a FOUR-byte span. Next owner at 0x02000038.
 * Takes no argument.
 *
 * Published, not called: its address is stored as the in-image spelling
 * 0x02008031 (offset + 0x8000, Thumb bit set) in exactly one word,
 * at overlay offset 0xc -- one field of a DESCRIPTOR TABLE filling this
 * overlay's first 0x30 bytes: five function pointers at 0xc, 0x14, 0x1c, 0x24
 * and 0x2c, stride 8, aimed at the five stubs beginning immediately after it
 * at 0x30. No `bl` anywhere in the image reaches any of them.
 *
 * SAME TABLE, SAME FIVE SLOTS, SAME FIVE BODIES as resource_37b -- and the
 * five ADDRESSES RETURNED ARE ALL DIFFERENT, which is the whole reason each
 * pool word is resolved per row instead of the shape being matched once.
 * 37b returns 0x0200a5c0/a698/a6bc/a80c; this overlay returns
 * 0x0200aafc/abec/ac14/ad34. Two overlays sharing a head layout is a fact
 * about the loader, not permission to copy an answer across.
 *
 * Reached by sweep B only: no `push` for C to key on and no `bl` for A,
 * so it lands in the "B leaf" class rather than the prologue-confirmed
 * population.
 *
 * Uncertainty: the returned address is a data pointer inside this overlay's
 * own image (0x2afc of 0x2ddc); what lives there is not established and
 * is deliberately not named.
 */

u8 *Func_02000030(void)
{
    return (u8 *)0x0200aafc;
}
