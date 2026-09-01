/* Contiguous unnamed leaf-owner run for resource_3b6. */

#include "types.h"

/*
 * resource_3b6 owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x2008dac.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008dac is image offset
 * 0xdac under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_0200003c(void)
{
    return (u8 *)0x02008dac;
}

#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_02000a14(s32);

s32 Func_02000044(void) {
    if (Func_02000a14(0x950) != 0) {
        return 0x02009040;
    }
    return 0x02008E00;
}

#include "types.h"

extern u8 Data_020099d0[];
extern u8 Data_02009670[];
extern u8 Data_02009310[];

extern s32 Func_02000a34(s32);
extern s32 Func_02000a42(s32);

u8 *Func_02000064(void)
{
    if (Func_02000a34(0x950) != 0) {
        return Data_020099d0;
    }
    if (Func_02000a42(0x962) != 0) {
        return Data_02009670;
    }
    return Data_02009310;
}

