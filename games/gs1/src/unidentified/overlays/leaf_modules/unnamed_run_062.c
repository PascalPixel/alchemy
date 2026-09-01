/* Contiguous unnamed leaf-owner run for resource_3c2. */

#include "types.h"

/*
 * resource_3c2 owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x2008c3c.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008c3c is image offset
 * 0xc3c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_02000030(void)
{
    return (u8 *)0x02008c3c;
}

#include "types.h"

/*
 * resource_3c2 owner at 0x02000038, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x200003c holding 0x2008da4.
 *
 * LEAF RESIDUE. Published at image offset 0x2c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000038 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008da4 is image offset
 * 0xda4 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_02000038(void)
{
    return (u8 *)0x02008da4;
}

#include "types.h"

/*
 * resource_3c2 owner at 0x02000040, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000044 holding 0x2008dd4.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000040 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008dd4 is image offset
 * 0xdd4 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_02000040(void)
{
    return (u8 *)0x02008dd4;
}

#include "types.h"

s32 Func_02000b82(s32);

s32 Func_02000048(void) {
    if (Func_02000b82(0x96f) != 0) {
        return 0x02008f28;
    }
    return 0x02008e08;
}

