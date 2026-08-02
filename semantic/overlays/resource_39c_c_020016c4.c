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
 * Uncertainty: none of the six resolved callees are identified beyond call shape;
 * the per-iteration constants (id 39/51/105/106, stack pair 3-i/6, 1/6,
 * 2/4) are recorded as passed values, not decoded further.
 */

extern void Func_080f9010(s32 arg0);
extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern void Func_080091f0(s32, s32, s32);
extern void Func_0808a010(s32 arg0);
extern void Func_080091c0();
extern void Func_080091f8(void);

void Func_020016c4(s32 arg0)
{
    s32 i;
    s32 a = 41;
    s32 b = 40;
    s32 c = 2;

    Func_080f9010(219);

    for (i = 0; i <= 2; i++) {
        Func_08009180(b, 32, a, 32, 3 - i, 6);
        Func_08009180(39, 51, b, 32, 1, 6);
        Func_08009180(105, 51, i + 0x6a, 32, c, 4);

        if (arg0 != 0) {
            Func_080091f0(0x50000, 0x50000, 0x10000);
            Func_080091f0(-1, -1, 0xe666);
            Func_0808a010(arg0);
        }

        a += 2;
        b += 2;
    }

    Func_080f9010(288);
    Func_080091c0(106, 33, 4, 5);
    Func_080091f8();
}
