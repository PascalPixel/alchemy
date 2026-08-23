#include "colosso_log_rolling_stage.h"

/*
 * resource_3bc owner at 0x02003a58, 98 bytes: forward the caller's three
 * arguments to sibling item-28 owner Func_020038fc, seed five of the
 * 0x0200391c task's state words directly from them, and install the task --
 * a second, more direct seeder for the same task that
 * start_palette_task_from_state.c installs by copying from two of these same
 * globals (Data_0200dc34, Data_0200dbc0) plus a third
 * (Data_0200dbcc, here reset to 0 rather than seeded from mode).
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
 * cover it -- resolved with `bun tools/overlay-show resource_3bc 3a58 |
 * cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3bc 3a58 3ab8 --annotate`):
 *   0x2003a64 -> raw prologue -> Func_020038fc(first_value, second_value, mode)
 *   0x2003a8a -> task-installer veneer with callback 0x0200b91d and budget 0xc80
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

void Func_02007362();          /* sibling item-28 owner, via per-site veneer */
void Func_020082d4(s32 taskAddress, s32 frameBudget);

void ColossoLogRollingStage_StartPaletteTask(u32 first_value, u32 second_value, u32 mode)
{
    Func_02007362(first_value, second_value, mode);

    Data_0200dc34 = (u16)first_value;
    Data_0200dbc0 = (u16)second_value;
    Data_0200db98 = (u16)(mode & 3);
    Data_0200dbb4 = 0;
    Data_0200dbcc = 0;

    {
        s32 budget = 0xc80;
        s32 task = 0x0200b91d;
        Func_020082d4(task, budget);
    }
}
