typedef int s32;

/*
 * resource_3bb owner at 0x02001ffc, 156 bytes: run a short setup pair,
 * then branch on two lookups off `arg1` to either return a status code
 * (2, 3, or a raw sub-result) or run a second closing sequence and
 * return whatever it produces.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02001ffc through `pop
 * {r5, r6} / pop {r1} / bx r1` at 0x02002094-0x02002098 -- the popped
 * word goes to r1, not r0, so r0 IS the result on every path, matching
 * this project's documented real-return-value convention
 * (resource_3cb_c_02000580.c). The six-word literal pool
 * 0x0200209c-0x020020b3 follows; the next owner's prologue is at
 * 0x020020b4 (this overlay's resource_3bb_c_020020b8.c, whose own id
 * table -- 0x8f/0x90/else -> 0x2076/0x2078/0x207a -- is the same one
 * this owner uses). Two arguments (r0, r1), `s32` return.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * Uncertainty: none of the eleven callees are identified beyond call
 * shape; the closing branch's return value is the un-moved return of
 * `Func_02005fda(0, 0)`, kept as a direct chained call rather than
 * introducing a named intermediate, since the compiled code never gives
 * it one either.
 */

extern void Func_02006084(s32 arg0);
extern void Func_02005ebc(s32 arg0, s32 arg1);
extern void Func_02006002(s32 arg0);
extern void Func_0200601a(s32 arg0, s32 arg1);
extern s32 Func_02005f2c(s32 arg0);
extern s32 Func_02005f3c(s32 arg0);
extern s32 Func_02005f1e(s32 arg0);
extern void Func_02005f6c(s32 arg0);
extern void Func_02006052(s32 arg0);
extern void Func_02006062(s32 arg0, s32 arg1);
extern s32 Func_02005fda(s32 arg0, s32 arg1);

s32 Func_02001ffc(s32 arg0, s32 arg1)
{
    short status = *(short *)0x02000400;
    s32 id;
    s32 result;

    Func_02006084(arg0);
    Func_02005ebc(arg1, 5);

    if (status == 0x8f) {
        id = 0x2076;
    } else if (status == 0x90) {
        id = 0x2078;
    } else {
        id = 0x207a;
    }
    Func_02006002(id);

    Func_0200601a(arg0, 0);

    if (Func_02005f2c(arg1 + 0x200) != 0) {
        return 2;
    }

    if (Func_02005f3c(arg1 + 0x208) == 0) {
        Func_02005f6c(arg1 + 0x208);
        Func_02006052(0x207c);
        Func_02006062(arg0, 0);
        return Func_02005fda(0, 0);
    }

    result = Func_02005f1e(0);
    if (result == 1) {
        return 2;
    }
    if (result == 2 || result == -1) {
        return 3;
    }
    return result;
}
