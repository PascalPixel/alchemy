#include "types.h"

/*
 * resource_3bc owner at 0x02003a58, 98 bytes: forward the caller's three
 * arguments to sibling item-28 owner Func_020038fc, seed five of the
 * 0x0200391c task's state words directly from them, and install the task --
 * a second, more direct seeder for the same task that
 * resource_3bc_c_02003abc.c installs by copying from two of these same
 * globals (Data_0200dc34, Data_0200dbc0) plus a third
 * (Data_0200dbcc, here reset to 0 rather than seeded from arg2).
 *
 * Complete owner: `push {r5, r6, lr} / mov r6,r8 / push {r6}` at 0x02003a58
 * through the matching interworking return `pop {r3} / mov r8,r3 /
 * pop {r5, r6} / pop {r0} / bx r0` at 0x02003ab0-0x02003ab8, followed by the
 * eight-word literal pool 0x02003a90-0x02003aab that closes the 98-byte
 * span (the `b.n 0x2003ab0` at 0x2003a8e branches over it, same mid-owner
 * shape used throughout this queue); the span ends at 0x02003aba, right
 * after the `bx r0`, with no trailing gap before the next owner.  Three
 * arguments (r0-r2), void.
 *
 * Not found by the structural inventory walk (unindexed); two call sites,
 * both resolved with the `+2` rule against the raw image (this owner starts
 * outside a recorded row, so overlay_call_targets.ts's own table does not
 * cover it -- resolved with `bun tools/overlay_show.ts resource_3bc 3a58 |
 * bun tools/overlay_call_targets.ts resource_3bc 3a58 3ab8 --annotate`):
 *   0x2003a64 -> raw prologue -> Func_020038fc(arg0, arg1, arg2)   sibling item-28 owner, same overlay, undrafted at time of writing
 *   0x2003a8a -> veneer -> Func_080000d0(0x0200b91d, 0xc80)   established task installer (resource_3bc_c_02003abc.c and others); same callback and rate as that sibling, confirming both seed the SAME task under the overlay's proven 0x02008000 link base (0x0200b91d - 1 - 0x02008000 = 0x391c)
 *
 * Uncertainty: the same as the 0x02003abc sibling -- these state words are
 * not otherwise cross-referenced from this owner alone, so their roles
 * beyond "task 0x0200391c's per-instance state" are not established.
 */

extern u16 Data_0200dc34;
extern u16 Data_0200dbc0;
extern u16 Data_0200db98;
extern u16 Data_0200dbb4;
extern u16 Data_0200dbcc;

void Func_020038fc();          /* sibling item-28 owner, same overlay */
void Func_080000d0(s32 taskAddress, s32 frameBudget);

void Func_02003a58(u32 arg0, u32 arg1, u32 arg2)
{
    Func_020038fc(arg0, arg1, arg2);

    Data_0200dc34 = (u16)arg0;
    Data_0200dbc0 = (u16)arg1;
    Data_0200db98 = (u16)(arg2 & 3);
    Data_0200dbb4 = 0;
    Data_0200dbcc = 0;

    Func_080000d0(0x0200b91d, 0xc80);
}
