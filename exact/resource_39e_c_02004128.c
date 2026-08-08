#include "types.h"

/*
 * resource_39e owner at 0x02004128, 24 bytes: three-call closer --
 * Func_02008584(0, 1), then the paired Func_02008672(2) /
 * Func_0200868e() imports. Called 3x within this overlay.
 *
 * Complete owner: `push {lr}` at 0x02004128 through `pop {r0} / bx r0`
 * at 0x200413c-0x200413e, no literal pool; the next owner's prologue
 * (`push {r5, r6, r7, lr}`, the 0x02004140 candidate) follows
 * immediately. No incoming arguments read, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_39e 4128 4140's
 * +2 rule).
 *
 * Uncertainty: the three callees are identified by call shape only.
 */

extern void Func_02008584(s32 id, s32 arg1);
extern void Func_02008672(s32 arg0);
extern void Func_0200868e(void);

void Func_02004128(void)
{
    Func_02008584(0, 1);
    Func_02008672(2);
    Func_0200868e();
}
