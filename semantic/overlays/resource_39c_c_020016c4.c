typedef int s32;

/*
 * resource_39c owner at 0x020016c4, 204 bytes: three-iteration setup loop,
 * each iteration issuing three fixed-shape calls plus (only when the
 * caller's argument is non-zero) three more, then two closing calls.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl spill at
 * 0x020016c4 through `pop {r3, r5, r6} / mov r8,r3 / mov r9,r5 / mov
 * sl,r6 / pop {r5, r6, r7} / pop {r0} / bx r0` at 0x0200177e-0x0200178a,
 * followed by the one-word literal pool 0x0200178c-0x0200178f; the next
 * owner's prologue is at 0x02001790 (a small two-instruction owner not in
 * this mandate's unindexed list, already covered elsewhere). One
 * argument (r0), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * Uncertainty: none of the nine callees are identified beyond call shape;
 * the per-iteration constants (id 39/51/105/106, stack pair 3-i/6, 1/6,
 * 2/4) are recorded as passed values, not decoded further.
 */

extern void Func_0200733a(s32 arg0);
extern void Func_02007100(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02007114(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_0200712a(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02007176(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02007184(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020071fa(s32 arg0);
extern void Func_020073c6(s32 arg0);
extern void Func_02007192(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_020071be();

void Func_020016c4(s32 arg0)
{
    s32 i;
    s32 a = 41;
    s32 b = 40;
    s32 c = 2;

    Func_0200733a(219);

    for (i = 0; i <= 2; i++) {
        Func_02007100(b, 32, a, 32, 3 - i, 6);
        Func_02007114(39, 51, b, 32, 1, 6);
        Func_0200712a(105, 51, i + 0x6a, 32, c, 4);

        if (arg0 != 0) {
            Func_02007176(0x50000, 0x50000, 0x10000);
            Func_02007184(-1, -1, 0xe666);
            Func_020071fa(arg0);
        }

        a += 2;
        b += 2;
    }

    Func_020073c6(288);
    Func_02007192(106, 33, 4, 5);
    Func_020071be();
}
