typedef int s32;

/*
 * resource_3b9 owner at 0x02001c48, 12 bytes: two-call trampoline,
 * forwarding `arg0` to the first callee with a fixed second argument,
 * then a fixed-constant second call.
 *
 * Complete owner: `push {lr}` at 0x02001c48 through `pop {r0} / bx r0`
 * at 0x02001c56-0x02001c58, followed by one alignment halfword; the
 * next owner's prologue is at 0x02001c5c (this overlay's own
 * resource_3b9_c_02001c5c.c). `arg0` is never freshly loaded before the
 * first call, so it is a forwarded pass-through parameter; void return.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2`
 * rule -- the single highest call count in this mandate's sweep so far
 * (75 sites).
 *
 * Uncertainty: neither callee is identified beyond call shape.
 */

extern void Func_020048f2(s32 arg0, s32 arg1);
extern void Func_02004840(s32 arg0);

void Func_02001c48(s32 arg0)
{
    Func_020048f2(arg0, 0);
    Func_02004840(10);
}
