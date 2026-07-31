#include "types.h"

/*
 * Clear 64 bytes at the given address and leave the pointer one record on.
 *
 * The image spells this as four unrolled store-multiples of four zeroed
 * registers through a writeback base, which is the shape of a fixed-size
 * record wipe rather than of a general memset: 64 bytes is one sound channel
 * record, and the base register is left addressing the next one.
 *
 * The routine is not a leaf convention problem — it borrows one callee-saved
 * register as its fourth zero and restores it before returning — but it never
 * touches the stack, so the tail branch at 0x080f99dc can reach it with a live
 * caller frame.  Expressed here as a word-granular clear, which is what the
 * store-multiples do; the caller-visible pointer advance is returned so that
 * the writeback is not lost.
 */

u32 *Func_080f9a18(u32 *record)
{
    int index;

    for (index = 0; index < 16; index++)
        *record++ = 0;

    return record;
}
