typedef signed int s32;

/*
 * resource_3b9 owner at 0x02000070, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * A LEAF, and one of the four rows that proved the closure standard was
 * incomplete. It is published -- the overlay's own header table at image
 * offset 0x2c holds 0x02008071, which is this offset with the Thumb bit
 * under the base + 0x8000 spelling -- so sweep B resolved it correctly
 * and then DISCARDED it, because until 2026-08-01 that sweep required a
 * `push` prologue at the target and a leaf has none. resource_3b9 was
 * certified closed at residue 0 with this row sitting in it.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees. `bx lr` rather than the interworking
 * `pop {r0} / bx r0` this overlay's larger rows use, because nothing was
 * pushed to pop.
 *
 * Bounds are exact and need no measurement: `AlchemyC_02000030` runs
 * `.space 0x40` to 0x02000070, and the next raw word at 0x02000074 is
 * this row's sibling getter Func_02000074.
 *
 * The header entry that publishes it is the fifth of six, and the sixth
 * publishes 0x02000074. A pair of four-byte stubs at the head of the
 * overlay, published from its own header, appears on nearly every
 * overlay in the tree.
 *
 * The return value is 0 and the row takes no argument that it reads --
 * r0 is written before any use, and nothing else is touched.
 */

s32 Func_02000070(void)
{
    return 0;
}
