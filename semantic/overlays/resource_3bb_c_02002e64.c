typedef unsigned short u16;
typedef int s32;

/*
 * resource_3bb owner at 0x02002e64, 30 bytes: refresh a cached cursor
 * value at 0x0200cb46 if it is unset -- the same
 * "if (cached == -1) cached = Func(...)" idiom
 * resource_3bb_c_020023bc.c already documents (there for a different
 * cell, 0x0200ca1c).
 *
 * Complete owner: `push {r5, lr}` at 0x02002e64 through `pop {r5} / pop
 * {r0} / bx r0` at 0x02002e7a-0x02002e7e, followed by one alignment
 * halfword; the next owner's prologue is at 0x02002e84. No incoming
 * arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * Uncertainty: `Func_02006d3e`'s role is not identified beyond call
 * shape.
 */

extern s32 Func_02006d3e(void);

void Func_02002e64(void)
{
    volatile short *cursor = (volatile short *)0x0200cb46;

    if (*cursor == -1) {
        *cursor = (u16)Func_02006d3e();
    }
}
