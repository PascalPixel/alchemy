/* Contiguous unnamed leaf-owner run for resource_375. */

#include "types.h"

/*
 * resource_375 owner at 0x02000094, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000098 holding 0x2009db4.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000094 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009db4 is image offset
 * 0x1db4 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

u8 *SceneData_GetTable9db4(void)
{
    return (u8 *)0x02009db4;
}

#include "types.h"

/*
 * resource_375 owner at 0x0200009c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x20000a0 holding 0x2009ddc.
 *
 * LEAF RESIDUE. Published at image offset 0x1c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200009c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009ddc is image offset
 * 0x1ddc under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

u8 *SceneData_GetTable9ddc(void)
{
    return (u8 *)0x02009ddc;
}

#include "types.h"

extern u8 Data_0200a028[];
extern u8 Data_02009fb0[];
extern u8 Data_02009efc[];

extern s32 Func_02001b0e(s32);
extern s32 Func_02001b1c(s32);

u8 *SceneData_SelectTable9efcByFlags(void)
{
    if (Func_02001b0e(0x87A) != 0) {
        return Data_0200a028;
    }
    if (Func_02001b1c(0x834) != 0) {
        return Data_02009fb0;
    }
    return Data_02009efc;
}
