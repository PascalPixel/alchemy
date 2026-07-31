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
 * Uncertainty: none of the seven callees are identified beyond call
 * shape. `Func_02007040`'s third argument (r2) is only ever set as a copy
 * of the same value moved into r0 immediately before -- kept as a
 * genuine (duplicate-valued) argument to match the two sibling calls'
 * four-register shape in the same loop body, but it could equally be a
 * stale leftover the real callee never reads; not resolved either way.
 * `Func_020070c8`'s closing call (after the loop) uses six arguments
 * where its in-loop call uses three -- declared old-style rather than
 * miscredited either shape, per this project's "arities vary per site"
 * convention (resource_3cb_c_02000580.c).
 */

extern void Func_02007262(s32 arg0);
extern void Func_0200702a(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_02007040(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_0200705a(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_0200706e(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_020070ba(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020070c8();
extern void Func_0200713e(s32 arg0);

void Func_020015e0(s32 arg0, s32 arg1, s32 arg2)
{
    s32 i;

    if (arg0 != 0) {
        Func_02007262(219);
    }

    for (i = arg1; i < arg2; i++) {
        Func_0200702a(45 - i * 2, 32, 44 - i * 2, 32, i + 1, 6);
        Func_02007040(45 - i, 51, 45 - i, 32, 1, 6);
        Func_0200705a(109 - i, 32, 108 - i, 32, 1, 4);
        Func_0200706e(109 - i, 51, 109 - i, 32, 1, 4);

        if (arg0 != 0) {
            Func_020070ba(0x50000, 0x50000, 0x10000);
            Func_020070c8(-1, -1, 0xe666);
            Func_0200713e(arg0);
        }
    }

    Func_020070c8(42, 52, 4, 5, 42, 33);
}
