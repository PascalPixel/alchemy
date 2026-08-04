#include "types.h"

/*
 * resource_37a owner at 0x02000038, 4 bytes: a return-zero stub --
 * `movs r0, #0 / bx lr`, the whole function.
 *
 * Complete owner: NO PROLOGUE, no stack, NO LITERAL POOL, lr never written,
 * bare `bx lr` at 0x0200003a. FOUR bytes, not eight, and that is the
 * point: its four neighbours in the same table are 8 because they carry a
 * pool word past the `bx lr`, and a constant that fits in an immediate
 * carries none. Next owner at 0x0200003c. Takes no argument.
 *
 * Published, not called: its address is stored as the in-image spelling
 * 0x02008039 (offset + 0x8000, Thumb bit set) in exactly one word,
 * at overlay offset 0x2c -- one field of a DESCRIPTOR TABLE filling this
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
 * Uncertainty: whether the caller reads the zero as a null pointer or as an
 * integer is not established, so the return is left as the widest thing the
 * row proves -- a word set to zero.
 */

s32 Func_02000038(void)
{
    return 0;
}
