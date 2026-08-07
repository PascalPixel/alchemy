#include "types.h"

/*
 * resource_3bb owner at 0x02001b30, 120 bytes: three groups, each
 * looking up two fixed-id values, packing each into a 12.20 fixed-point
 * word with a shared 0x80000 offset, then handing the pair to a
 * per-group "set position" callee tagged 1, 2, 3.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02001b30 through `pop
 * {r5, r6} / pop {r0} / bx r0` at 0x02001ba2-0x02001ba6; no literal pool,
 * the next owner's prologue is immediately at 0x02001ba8. No incoming
 * arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/lib/overlay_call_targets.ts`'s `+2` rule.
 *
 * Uncertainty: none of the nine callees are identified beyond call
 * shape; the six looked-up ids (896, 904, 912, 920, 928, 936, each 8
 * apart -- the same stride resource_3bb_c_02001fb8.c's six-call family
 * uses) are recorded as passed values.
 */




extern s32 Func_02005a38(s32 id);
extern s32 Func_02005a42(s32 id);
extern void Func_02005ae8(s32 tag, s32 a, s32 b);
extern s32 Func_02005a60(s32 id);
extern s32 Func_02005a6a(s32 id);
extern void Func_02005b0c(s32 tag, s32 a, s32 b);
extern s32 Func_02005a84(s32 id);
extern s32 Func_02005a8e(s32 id);
extern void Func_02005b30(s32 tag, s32 a, s32 b);
void Func_02001b30(void)
{
    s32 a, b;

    a = (Func_02005a38(896) << 20) + 0x80000;
    b = (Func_02005a42(904) << 20) + 0x80000;
    Func_02005ae8(1, a, b);

    a = (Func_02005a60(912) << 20) + 0x80000;
    b = (Func_02005a6a(920) << 20) + 0x80000;
    Func_02005b0c(2, a, b);

    a = (Func_02005a84(928) << 20) + 0x80000;
    b = (Func_02005a8e(936) << 20) + 0x80000;
    Func_02005b30(3, a, b);
}
