typedef unsigned char u8;

/*
 * resource_3aa owner at 0x02000064, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000068 holding 0x2009d9c.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000064 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009d9c is image offset
 * 0x1d9c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

extern u8 Data_002009d9c[];   /* image offset 0x1d9c */

u8 *Func_02000064(void)
{
    return Data_002009d9c;
}
