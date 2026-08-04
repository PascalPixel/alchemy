#include "types.h"

/*
 * resource_39c owner at 0x02004f90, 60 bytes: two chained lookups against
 * `arg0`, then a halfword store into a table keyed by the second lookup's
 * result.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02004f90 through
 * `pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x2004fc2-0x2004fca; no literal pool, the next owner's prologue is
 * immediately at 0x02004fcc.  Two arguments (r0, r1), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * Both early-out branches compare against -1, a shared sentinel between
 * Func_08077040 and Func_08077038.
 *
 * Uncertainty: none of the three callees are identified beyond their call
 * shape; the table Func_08077008 returns is not identified either.
 */

extern s32 Func_08077040(s32 arg0);
extern s32 Func_08077038(s32 arg0, s32 arg1);
extern u8 *Func_08077008(s32 arg0);

void Func_02004f90(s32 arg0, u16 arg1)
{
    s32 a;
    s32 b;
    u8 *table;

    a = Func_08077040(arg0);
    if (a == -1) {
        return;
    }

    b = Func_08077038(a, arg0);
    if (b == -1) {
        return;
    }

    table = Func_08077008(a);
    *(u16 *)(table + b * 2 + 216) = arg1;
}
