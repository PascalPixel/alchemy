#include "types.h"

#define Resource37c_Run Func_02000054

#include "resource_37c.h"
#include "resource_37c_runtime.h"

/*
 * resource_37c owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x20080c8.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20080c8 is image offset
 * 0xc8 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_37c owner at 0x02000038, 4 bytes: `movs r0, #0 / bx lr`.
 *
 * LEAF RESIDUE. Published at image offset 0x2c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 * A leaf never does -- it saves no register and returns with `bx lr`.
 *
 * Complete owner: both instructions. No prologue, no stack frame, no
 * literal pool, no callees, no argument read.
 *
 * One of the 70 rows sharing this exact body across the tree. The body is
 * shared; the identity is not -- this row is bounded by ITS overlay's
 * neighbours and published from ITS overlay's table.
 */

/* Contiguous unnamed leaf-owner run for resource_37c. */

/*
 * resource_37c owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x2008110.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008110 is image offset
 * 0x110 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_37c owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000048 holding 0x200811c.
 *
 * LEAF RESIDUE. Published at image offset 0x1c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000044 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200811c is image offset
 * 0x11c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_37c owner at 0x0200004c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000050 holding 0x2008134.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200004c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008134 is image offset
 * 0x134 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

u8 *Func_02000030(void)
{
    return RESOURCE37C_PRIMARY_TABLE;   /* image offset 0xc8 */
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *Func_0200003c(void)
{
    return RESOURCE37C_SECONDARY_TABLE;   /* image offset 0x110 */
}

u8 *Func_02000044(void)
{
    return RESOURCE37C_TERTIARY_TABLE;   /* image offset 0x11c */
}

u8 *Func_0200004c(void)
{
    return RESOURCE37C_QUATERNARY_TABLE;   /* image offset 0x134 */
}

s32 Resource37c_Run(void)
{
    /* Keep distinct entry-block 16.16 scale locals: local CSE otherwise
       replaces two materializations with register copies. */
    s32 scale_x = 0x10000;
    s32 scale_y = 0x10000;
    s32 scale_z = 0x10000;

    RESOURCE37C_RUNTIME->setup_request_1c0 = 0x204;
    Resource37c_SetSetupFlag(0x144);
    if (Resource37c_TestSetupGate(0x814) != 0) {
        Resource37c_QueueSoundCue(0x8D);
        Resource37c_SetViewScale(scale_x, scale_y, scale_z);
        Resource37c_FinalizeSetup();
    }
    return 0;
}
