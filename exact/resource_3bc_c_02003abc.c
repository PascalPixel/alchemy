#include "types.h"

/*
 * resource_3bc owner at 0x02003abc, 92 bytes: seed the per-instance state
 * word block for the 0x0200391c task and install it.
 *
 * Complete owner: `push {lr}` at 0x02003abc through `pop {r0} / bx r0` at
 * 0x02003aea, followed by the ten-word literal pool 0x02003aee-0x02003b17
 * that closes the 92-byte span; the next owner's prologue is at 0x02003b18.
 * Three-argument (r0-r2), void: the popped word is the return address.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/lib/overlay_call_targets.ts`'s `+2` rule against
 * the raw image (the tool's own site table does not cover this owner since
 * it starts outside a recorded row) -- the one call site,
 * 0x02003ae6 -> veneer 0x02004848 -> Func_080000d0, the task installer this
 * overlay's siblings already use (see resource_36f_c_02000238.c's header).
 *
 * The installed callback, 0x0200b91d under this overlay's proven 0x02008000
 * link base, is file offset 0x391d minus the Thumb bit: this overlay's own
 * 0x0200391c, a substantial multi-register owner outside this mandate's
 * called-function list (it is never reached by `bl`, only installed as a task
 * pointer, so `overlay_unindexed.ts`'s scan does not see it either). Named as
 * the raw link-base literal per HANDOVER's address-taken rule, matching the
 * sibling's own `Func_080000d0(0x02008239, 3200)` spelling rather than a
 * symbolic name.
 *
 * Uncertainty: the five state words (Data_0200dba0/dc40/dc34/dbe4/dbc0/dbfc/
 * dbcc/db90) are not otherwise cross-referenced from this owner alone; their
 * roles are inferred only from this store/copy/zero pattern (three fresh
 * arguments, two carried-over values, one reset counter) and the identical
 * shape of the sibling installers already tracked in this overlay.
 */

extern u16 Data_0200dba0;
extern u16 Data_0200dc40;
extern u16 Data_0200dc34;
extern u16 Data_0200dbe4;
extern u16 Data_0200dbc0;
extern u16 Data_0200dbfc;
extern u16 Data_0200dbcc;
extern u16 Data_0200db90;

void Func_02008330(s32 taskAddress, s32 frameBudget);

void Func_02003abc(u32 arg0, u32 arg1, u32 arg2)
{
    Data_0200dba0 = (u16)arg0;
    Data_0200dc40 = (u16)arg1;
    Data_0200dbe4 = Data_0200dc34;
    Data_0200dbfc = Data_0200dbc0;
    Data_0200dbcc = (u16)arg2;
    Data_0200db90 = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_02008330(task, budget);
    }
}
