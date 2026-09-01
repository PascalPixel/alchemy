/* Contiguous unnamed leaf-owner run for resource_3b7. */

#include "types.h"

/*
 * resource_3b7 owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x2009ac8.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009ac8 is image offset
 * 0x1ac8 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_0200003c(void)
{
    return (u8 *)0x02009ac8;
}

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000bd;
extern u8 Data_02009aec[];
extern u8 Data_02009cfc[];

s32 Func_02000044(void) {
    if (Data_02000240[224] == (s32)&Value_000000bd) {
        return (s32)Data_02009aec;
    }
    return (s32)Data_02009cfc;
}

