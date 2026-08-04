#include "types.h"

/*
 * resource_3b9 owner at 0x02001c6c, 100 bytes: check a fixed status id;
 * if set, run two 6-argument setup calls; otherwise check a second
 * status id and, if that one is also set, run one more 6-argument
 * setup call.
 *
 * Complete owner: `push {lr}` at 0x02001c6c through `pop {r0} / bx r0`
 * at 0x02001ccc-0x02001cce, followed by the one-word literal pool
 * 0x02001cd0-0x02001cd3; the next owner's prologue is at 0x02001cd4
 * (this overlay's own resource_3b9_c_02001cd4.c candidate, not yet
 * drafted). No incoming arguments are read before being overwritten,
 * so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule.
 *
 * Uncertainty: none of the four resolved callees are identified beyond call
 * shape; the ids (0x950, 16, 0x962) and constant argument pairs are
 * recorded as passed values.
 */

extern s32 Func_080770c0(s32 id);
extern void Func_08009180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern s32 Func_0808a158(s32 arg0, s32 arg1);

void Func_02001c6c(void)
{
    if (Func_080770c0(0x950) != 0) {
        Func_08009180(64, 0, 48, 5, 2, 2);
        Func_080091c0(14, 8, 2, 1, 16, 8);
    } else {
        Func_0808a158(16, 2);
        if (Func_080770c0(0x962) != 0) {
            Func_080091c0(30, 22, 1, 2, 14, 11);
        }
    }
}
