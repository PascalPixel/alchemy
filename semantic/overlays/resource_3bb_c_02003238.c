typedef int s32;

/*
 * resource_3bb owner at 0x02003238, 36 bytes: spin-wait until a status
 * cell reaches 9, calling a poll/tick callee once per iteration.
 *
 * Complete owner: `push {r5, lr}` at 0x02003238 through `pop {r5} / pop
 * {r0} / bx r0` at 0x02003252-0x02003256, followed by the one-word
 * literal pool 0x02003258-0x0200325b; the next owner's prologue is at
 * 0x0200325c. No incoming arguments are read before being overwritten,
 * so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * Uncertainty: `Data_02001000`'s role and `Func_02006fe8`'s are not
 * identified beyond this call shape.
 */

extern void Func_02006fe8(s32 arg0);

void Func_02003238(void)
{
    volatile short *status = (volatile short *)0x02001000;

    while (*status != 9) {
        Func_02006fe8(1);
    }
}
