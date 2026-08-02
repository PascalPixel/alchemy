typedef int s32;

/*
 * resource_39c owner at 0x020015e0, 228 bytes: for each index in
 * `[arg1, arg2)`, issue four fixed-shape setup calls (three of them with
 * a mirrored `id`/`id2` pair computed from the loop index, the fourth
 * gated on `arg0`), then one closing call after the loop.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill at
 * 0x020015e0 through `pop {r3, r5, r6, r7} / mov r8,r3 / mov r9,r5 / mov
 * sl,r6 / mov fp,r7 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x020016b0-0x020016be, followed by the one-word literal pool
 * 0x020016c0-0x020016c3; the next owner's prologue is at 0x020016c4
 * (this overlay's own resource_39c_c_020016c4.c). Three arguments
 * (r0-r2), void.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * Uncertainty: none of the five resolved callees are identified beyond call
 * shape. `Func_08009180`'s third argument (r2) is only ever set as a copy
 * of the same value moved into r0 immediately before -- kept as a
 * genuine (duplicate-valued) argument to match the two sibling calls'
 * four-register shape in the same loop body, but it could equally be a
 * stale leftover the real callee never reads; not resolved either way.
 * The closing six-argument dispatch is `Func_080091c0`; the optional
 * three-argument setup calls resolve separately to `Func_080091f0`.
 */

extern void Func_080f9010(s32 arg0);
extern void Func_08009180(s32, s32, s32, s32, s32, s32);
extern void Func_080091f0(s32, s32, s32);
extern void Func_0808a010(s32 arg0);
extern void Func_080091c0(s32, s32, s32, s32, s32, s32);

void Func_020015e0(s32 arg0, s32 arg1, s32 arg2)
{
    s32 i;

    if (arg0 != 0) {
        Func_080f9010(219);
    }

    for (i = arg1; i < arg2; i++) {
        Func_08009180(45 - i * 2, 32, 44 - i * 2, 32, i + 1, 6);
        Func_08009180(45 - i, 51, 45 - i, 32, 1, 6);
        Func_08009180(109 - i, 32, 108 - i, 32, 1, 4);
        Func_08009180(109 - i, 51, 109 - i, 32, 1, 4);

        if (arg0 != 0) {
            Func_080091f0(0x50000, 0x50000, 0x10000);
            Func_080091f0(-1, -1, 0xe666);
            Func_0808a010(arg0);
        }
    }

    Func_080091c0(42, 52, 4, 5, 42, 33);
}
