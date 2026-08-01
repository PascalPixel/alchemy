typedef unsigned char u8;

/*
 * resource_resource_3a5 owner at 0x02000030, 22 bytes. No literal pool.
 *
 * HEAD LEAF. It lies BEFORE the first recorded owner (0x02000048), in the
 * region `gapsBetween` never read until 2026-08-01 -- it paired owner N with
 * owner N+1 and so skipped everything ahead of owner 0. It opens with no
 * `push`, so sweep C is blind; on this overlay no `bl` targets it and no word
 * publishes it, so sweeps A and B are blind too. Invisible to all four.
 *
 * The ROM, verbatim:
 *
 *   ldr  r0, [r0, #80]
 *   movs r3, #3
 *   ldrb r2, [r0, #9]
 *   ands r1, r3
 *   movs r3, #13
 *   negs r3, r3        ; r3 = -13 = 0xfffffff3 = ~0x0c
 *   lsls r1, r1, #2
 *   ands r3, r2
 *   orrs r3, r1
 *   strb r3, [r0, #9]
 *   bx   lr
 *
 * A two-bit BITFIELD WRITE at bits 2-3 of the byte at +9 of the record held at
 * +80 of the argument. `negs r3, r3` on 13 yields ~0x0c and not ~0x0d, which is
 * the whole reason the mask is worth writing out: the immediate in the ROM is
 * one greater than the mask it produces, and reading it as `~13` would clear
 * the wrong bit.
 *
 * SPAN IS 22 BYTES. There is no pool to annex -- every constant is an
 * immediate -- and the halfword at 0x2000046 is zero alignment before the first
 * owner at 0x2000048, which is a 2-byte remainder and therefore alignment by
 * this tree's own ALIGNMENT_SLACK rule, not an orphaned word.
 *
 * BYTE-IDENTICAL ON FOURTEEN OVERLAYS -- 382, 385, 387, 38a, 396, 39b, 39c,
 * 3a0, 3a5, 3a6, 3ab, 3b3, 3be, 3c0 -- each at 0x30 with its first owner at
 * 0x48. Shared shape is normally not permission to carry an answer across, and
 * the reason it IS permission here is specific and checkable: THIS FUNCTION HAS
 * NO LITERAL POOL. Every operand is an immediate encoded in the instruction, so
 * there is no per-overlay word for two copies to differ in -- unlike 37a/37b,
 * or the 3c9/3a1 pair at 0x200013c that differ in eleven halfwords and index
 * different tables. The claim is about THIS body's operand encoding, and it
 * does not generalise to the next identical-looking pair.
 *
 * ON resource_39c THE SAME FUNCTION HAS 28 CALL SITES. Here it has none. The
 * body being shared says nothing about who calls it, and an unreferenced copy
 * is not evidence that the referenced one is unreachable.
 *
 * 0 of 0 callees.
 */

void Func_02000030(void *record, u8 value)
{
    u8 *inner = *(u8 **)((u8 *)record + 80);

    inner[9] = (u8)((inner[9] & ~0x0c) | ((value & 3) << 2));
}
