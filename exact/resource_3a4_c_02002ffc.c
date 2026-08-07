#include "types.h"

/*
 * resource_3a4 owner at 0x02002ffc, 44 bytes: test bit 1 of the runtime
 * word at 0x03001e40 and call Func_0808a158 on record 8 with 7 or 6.
 * A PUBLISHED CALLBACK -- surfaced by sweep B of
 * tools/lib/overlay_published.ts, never the target of a `bl`.
 *
 * Complete owner: `push {lr}` at 0x02002ffc through the interworking
 * return `pop {r0} / bx r0` at 0x0200301e-0x02003020, one alignment
 * halfword, and the one-word literal pool 0x02003024-0x02003027; the
 * next owner begins at 0x02003028 and is itself unowned residue (sweep
 * A, bl-reached, 938 bytes, left mapped rather than drafted this
 * shift).  measureSpan reports 38; the pool makes it 44.
 *
 * Both calls resolve to the one veneer Func_0808a158 by targetOffset,
 * printed by the listing as two different addresses.  No arguments,
 * void: r0 is written before each call rather than passed in, which is
 * the contrast with the neighbouring 0x02002ce0 and is why that row
 * takes a parameter and this one does not.
 *
 * Uncertainty: 0x03001e40 is a runtime address outside this image and
 * bit 1 is read as a two-way selector whose meaning is open; it is the
 * same word 0x02002ce0 and 0x02002b58 read, each masking different
 * bits, so it looks like a packed status field shared across this
 * overlay's callbacks -- observed, not proven.  8, 7 and 6 are literal.
 */

extern u32 Data_03001e40;

extern void Func_02006c2a();
extern void Func_02006c34();


void Func_02002ffc(void)
{
    if (((Data_03001e40 >> 1) & 1) != 0) {
        Func_02006c2a(8, 7);
    } else {
        Func_02006c34(8, 6);
    }
}
