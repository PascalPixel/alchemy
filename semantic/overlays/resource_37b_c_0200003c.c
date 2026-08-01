typedef unsigned char u8;

/*
 * resource_37b owner at 0x0200003c, 8 bytes: a constant getter --
 * `ldr r0, [pc, #0] / bx lr` over its own one-word pool, returning the
 * in-image address 0x0200a698 (overlay offset 0x2698).
 *
 * Complete owner: NO PROLOGUE. It saves nothing, touches no stack, never
 * writes lr, and returns with a bare `bx lr` at 0x0200003e. The span
 * is 8 bytes and NOT 4: the literal pool word sits PAST the
 * `bx lr`, at 0x02000040, and belongs to this owner. Next owner at
 * 0x02000044. Takes no argument.
 *
 * Published, not called: its address is stored as the in-image spelling
 * 0x0200803d (offset + 0x8000, Thumb bit set) in exactly one word,
 * at overlay offset 0x14. That word is one field of a DESCRIPTOR TABLE
 * filling the overlay's first 0x30 bytes: five function pointers at offsets
 * 0xc, 0x14, 0x1c, 0x24 and 0x2c -- stride 8 -- aimed at the five stubs that
 * begin immediately after it at 0x30. No `bl` anywhere in the image reaches
 * any of them.
 *
 * INVISIBLE TO SWEEPS A AND C, and only sweep B reaches it: no `push`
 * for C to key on, and nothing calls it for A. Sweep B finds it because
 * it validates a published word by looking for a prologue -- and this
 * has none -- so it lands in the "B leaf" class rather
 * than in the prologue-confirmed population.
 *
 * IDENTICAL BYTES ARE NOT IDENTICAL SEMANTICS. Four of this overlay's
 * five head stubs share this exact two-instruction body and each returns
 * a DIFFERENT address, so the pool word is resolved per row rather than
 * the shape matched once: 0x02000030 -> 0x0200a5c0, 0x0200003c ->
 * 0x0200a698, 0x02000044 -> 0x0200a6bc, 0x0200004c ->
 * 0x0200a80c. The fifth, 0x02000038, is a different body
 * returning zero. A cohort is not a copy-paste.
 *
 * Uncertainty: the returned address is a data pointer inside this
 * overlay's own image (0x2698 of 0x2978); nothing here establishes
 * what lives there, and it is deliberately not named.
 */

u8 *Func_0200003c(void)
{
    return (u8 *)0x0200a698;
}
