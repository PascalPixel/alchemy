#include "types.h"

/*
 * resource_3bb owner at 0x02002380, 60 bytes: look up an object by
 * arg0, then scan the first 15 halfword entries of its table at offset
 * 0xd8 for one equal to arg1, calling a handler with the matching index.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8 spill at 0x02002380
 * through `pop {r3} / mov r8,r3 / pop {r5, r6, r7} / pop {r0} / bx r0`
 * at 0x020023b2-0x020023ba; no literal pool, the next owner's prologue
 * is immediately at 0x020023bc. Two arguments (r0, r1), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2` rule.
 *
 * Uncertainty: none of the three callees are identified beyond call
 * shape; the 15-entry table's role is inferred only from this scan.
 */

extern u16 *Func_0200625c(s32 arg0);
extern void Func_0200626e(s32 arg0, s32 arg1);
extern void Func_0200628a(s32 arg0, s32 index);

void Func_02002380(s32 arg0, s32 arg1)
{
    u16 *table = Func_0200625c(arg0);
    s32 i;

    Func_0200626e(arg0, arg1);

    table = (u16 *)((char *)table + 0xd8);
    for (i = 0; i <= 14; i++) {
        if (table[i] == arg1) {
            Func_0200628a(arg0, i);
        }
    }
}
