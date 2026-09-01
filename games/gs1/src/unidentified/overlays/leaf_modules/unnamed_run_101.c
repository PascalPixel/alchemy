/* Contiguous unnamed leaf-owner run for resource_3c7. */

#include "types.h"

/*
 * resource_3c7 owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000048 holding 0x2009438.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000044 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2009438 is image offset
 * 0x1438 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_02000044(void)
{
    return (u8 *)0x02009438;
}

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000b3;
extern u8 Data_02009690[];
extern u8 Data_020096b0[];

s32 Func_0200004c(void) {
    if (Data_02000240[224] == (s32)&Value_000000b3) {
        return (s32)Data_02009690;
    }
    return (s32)Data_020096b0;
}

#include "types.h"

/*
 * resource_3c7 owner at 0x0200007c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000080 holding 0x20096d0.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200007c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20096d0 is image offset
 * 0x16d0 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_0200007c(void)
{
    return (u8 *)0x020096d0;
}

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000b4;
extern u8 Data_02009734[];
extern u8 Data_02009974[];
extern u8 Data_0200989c[];

extern s32 Func_02001278(s32);

u8 *Func_02000084(void)
{
    if (Data_02000240[224] == (s32) &Value_000000b4) {
        if (Func_02001278(0x9A7) != 0) {
            return Data_02009974;
        }
        return Data_0200989c;
    }
    return Data_02009734;
}

