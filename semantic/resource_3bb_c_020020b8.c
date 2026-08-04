#include "types.h"

/*
 * resource_3bb owner at 0x020020b8, 92 bytes: pick one of three ids by
 * a shared status cell's value, then run a short two-call closing
 * sequence on `arg0`.
 *
 * Complete owner: `push {r5, lr}` at 0x020020b8 through `pop {r5} / pop
 * {r0} / bx r0` at 0x020020f4-0x020020f8, followed by the six-word
 * literal pool 0x020020fc-0x02002113; the next owner's prologue is at
 * 0x02002114 (this overlay's item-28 candidate at that address, not yet
 * drafted). Two arguments (r0, r1), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * Uncertainty: none of the three callees are identified beyond call
 * shape; the status cell (0x02000400, the same 0x02000240-relative
 * table this overlay's siblings elsewhere in the tree also index) and
 * the three ids (0x2076/0x2078/0x207a) are recorded as compared/passed
 * values.
 */

extern short Data_02000240[];
extern void Func_08015120(s32 arg0, s32 arg1);
extern void Func_0808a170(s32 arg0);
extern void Func_0808a180(s32 arg0, s32 arg1);

void Func_020020b8(s32 arg0, s32 arg1)
{
    short status;
    s32 id;

    Func_08015120(arg1, 5);
    status = Data_02000240[224];

    if (status == 0x8f) {
        id = 0x2076;
    } else if (status == 0x90) {
        id = 0x2078;
    } else {
        id = 0x207a;
    }

    Func_0808a170(id + 1);
    Func_0808a180(arg0, 0);
}
