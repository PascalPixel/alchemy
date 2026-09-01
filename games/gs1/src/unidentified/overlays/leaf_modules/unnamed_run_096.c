/* Contiguous unnamed leaf-owner run for resource_386. */

#include "types.h"

extern void Func_02000ae0(void);
extern s32 Func_02000abe(s32);
extern void Func_02000b48(s32);
extern void Func_02000b50(s32);
extern void Func_02000b68(s32, s32);
extern void Func_02000b0c(void);

void Func_020004a0(void)
{
    Func_02000ae0();
    if (Func_02000abe(0x85B) == 0) {
        Func_02000b48(0x1382);
    } else {
        Func_02000b50(0x1CF4);
    }
    Func_02000b68(18, 0);
    Func_02000b0c();
}

#include "types.h"

/*
 * resource_386 owner at 0x020004dc, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x20004e0 holding 0x200898c.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x020004dc reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200898c is image offset
 * 0x98c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

u8 *Func_020004dc(void)
{
    return (u8 *)0x0200898c;
}

