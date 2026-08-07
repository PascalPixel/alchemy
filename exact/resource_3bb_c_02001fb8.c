#include "types.h"

/*
 * resource_3bb owner at 0x02001fb8, 66 bytes: six calls to a family of
 * near-identical handler stubs (10 bytes apart, 0x2005eca..0x2005efc),
 * each with its own fixed id (896, 904, 912, 920, 928, 936 -- each 8
 * apart) and a second argument of 0.
 *
 * Complete owner: `push {lr}` at 0x02001fb8 through `pop {r0} / bx r0` at
 * 0x02001ff6-0x02001ff8, followed by one alignment halfword; the next
 * owner's prologue is at 0x02001ffc. No incoming arguments are read
 * before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/lib/overlay_call_targets.ts`'s `+2` rule.
 *
 * Uncertainty: none of the six callees are identified beyond call shape.
 */

extern void Func_02005eca(s32 arg0, s32 arg1);
extern void Func_02005ed4(s32 arg0, s32 arg1);
extern void Func_02005ede(s32 arg0, s32 arg1);
extern void Func_02005ee8(s32 arg0, s32 arg1);
extern void Func_02005ef2(s32 arg0, s32 arg1);
extern void Func_02005efc(s32 arg0, s32 arg1);

void Func_02001fb8(void)
{
    Func_02005eca(896, 0);
    Func_02005ed4(904, 0);
    Func_02005ede(912, 0);
    Func_02005ee8(920, 0);
    Func_02005ef2(928, 0);
    Func_02005efc(936, 0);
}
