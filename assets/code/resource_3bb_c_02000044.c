typedef unsigned char u8;

/*
 * resource_3bb owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr`
 * plus the one-word literal pool at 0x02000048 holding 0x200c48c.
 *
 * HEAD EXPORT STUB. Published from this overlay's own header word at image
 * offset 0x1c. It lies BEFORE the first recorded owner, in the region
 * `gapsBetween` never read until 2026-08-01, and it is visible to the head
 * sweep only because the veneer predicate compares registers: `ldr r0,=X /
 * bx lr` is a LEAF, not a veneer, and the old predicate masked every one of
 * these as structure.
 *
 * SPAN IS 8 BYTES, NOT 4. The pool word sits PAST the `bx lr` and is read by
 * this row's own `ldr r0, [pc, #0]` -- pc 0x02000048 after alignment, plus 0 --
 * so it belongs to this owner. Recording 4 would orphan a word and
 * manufacture a phantom gap.
 *
 * The pool word 0x200c48c is an ADDRESS, loaded and returned WITHOUT being
 * dereferenced, so the row is a getter for an in-image table. One of the 29
 * head getters on ten overlays, and 28 of the 29 pool words are distinct.
 * The one repeat is 0x2009c34, returned by BOTH resource_395 0x48 and
 * resource_3ad 0x30 -- and that is a coincidence of ADDRESS, not shared
 * identity: only one overlay is resident at 0x02000000 at a time, so the
 * same VA designates different bytes depending on which is loaded.
 * Identical bytes are not identical semantics.
 *
 * 0 of 0 callees.
 */


u8 *Func_02000044(void)
{
    return (u8 *)0x0200c48c;
}
