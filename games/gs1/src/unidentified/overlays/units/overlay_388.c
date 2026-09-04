#include "types.h"

#define Resource388_Run Func_02000054

#include "resource_388.h"
#include "resource_388_runtime.h"

/*
 * resource_388 owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x2008108.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008108 is image offset
 * 0x108 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_388 owner at 0x02000038, 4 bytes: `movs r0, #0 / bx lr`.
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

/* Contiguous unnamed leaf-owner run for resource_388. */

/*
 * resource_388 owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x2008180.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008180 is image offset
 * 0x180 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_388 owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000048 holding 0x2008194.
 *
 * LEAF RESIDUE. Published at image offset 0x1c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000044 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008194 is image offset
 * 0x194 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_388 owner at 0x0200004c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000050 holding 0x20081c4.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200004c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20081c4 is image offset
 * 0x1c4 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

u8 *Func_02000030(void)
{
    return RESOURCE388_PRIMARY_TABLE;   /* image offset 0x108 */
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *Func_0200003c(void)
{
    return RESOURCE388_SECONDARY_TABLE;   /* image offset 0x180 */
}

u8 *Func_02000044(void)
{
    return RESOURCE388_TERTIARY_TABLE;   /* image offset 0x194 */
}

u8 *Func_0200004c(void)
{
    return RESOURCE388_QUATERNARY_TABLE;   /* image offset 0x1c4 */
}

s32 Resource388_Run(void) {
    struct Resource388Runtime *runtime;
    /* slot_state_byte, command_arg5 and command_arg6 look gratuitous but are
       load-bearing: they keep the values out of the immediate operands of the
       stores/call so the allocator gives them their own registers. */
    s8 slot_state_byte;
    s32 command_arg5;
    s32 command_arg6;
    /* Likewise position_x/position_z: initialising them here (outside the
       call's basic block) stops the call expander from pre-copying the two
       24-bit constants ahead of the r0 argument. */
    s32 position_x = 0xD80000;
    s32 position_z = 0x880000;

    runtime = RESOURCE388_RUNTIME;
    runtime->setup_request_1c0 = 0x204;
    runtime->setup_value_1c8 = 0x18;
    if (Resource388_TestSetupGate(0x300) != 0) {
        Resource388_SetSlotPosition(8, position_x, position_z);
        Resource388_SetSlotMode(8, 2);
        Resource388_SetSlotOption(Resource388_LookupSlotHandle(8), 0);
        Resource388_GetSlotRecord(8)->unknown_23 = 2;
        slot_state_byte = 0;
        Resource388_GetSlotState(8)->unknown_59 = slot_state_byte;
        command_arg5 = 0xB;
        command_arg6 = 6;
        Resource388_QueueSlotCommand(0xB, 0x24, 5, 5, command_arg5, command_arg6);
    }
    return 0;
}
