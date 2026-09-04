#include "types.h"

#define Resource37d_Run Func_02000054

#include "resource_37d.h"
#include "resource_37de_scene.h"

/*
 * resource_37d owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x20080c0.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20080c0 is image offset
 * 0xc0 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_37d owner at 0x02000038, 4 bytes: `movs r0, #0 / bx lr`.
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

/* Contiguous unnamed leaf-owner run for resource_37d. */

/*
 * resource_37d owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x2008120.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008120 is image offset
 * 0x120 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_37d owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000048 holding 0x2008130.
 *
 * LEAF RESIDUE. Published at image offset 0x1c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000044 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008130 is image offset
 * 0x130 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_37d owner at 0x0200004c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000050 holding 0x2008148.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200004c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008148 is image offset
 * 0x148 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

u8 *Func_02000030(void)
{
    return RESOURCE37D_PRIMARY_TABLE;   /* image offset 0xc0 */
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *Func_0200003c(void)
{
    return RESOURCE37D_SECONDARY_TABLE;   /* image offset 0x120 */
}

u8 *Func_02000044(void)
{
    return RESOURCE37D_TERTIARY_TABLE;   /* image offset 0x130 */
}

u8 *Func_0200004c(void)
{
    return RESOURCE37D_QUATERNARY_TABLE;   /* image offset 0x148 */
}

s32 Resource37d_Run(void) {
    struct Resource37deSceneRuntime *runtime;
    /* The three scale values are locals, not literals at the call, so that
       their materialisation sits in the entry block instead of the call's.
       Local CSE only unifies identical large constants inside one basic
       block; folding these back into the argument list collapses the three
       movs/lsls pairs to one pair plus two register copies. */
    s32 camera_zoom_x = 0x10000;
    s32 camera_zoom_y = 0x10000;
    s32 camera_zoom_z = 0x10000;

    runtime = RESOURCE37DE_SCENE_RUNTIME;
    runtime->scene_request_1c0 = 0x204;
    runtime->scene_setup_word_1c8 = 0x10;
    if (TestSceneFlag(0x814) != 0) {
        QueueSceneSound(0x8D);
        SetCameraZoom(camera_zoom_x, camera_zoom_y, camera_zoom_z);
        InitializeSceneRecordBuffer();
    }
    return 0;
}
