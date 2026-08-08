#include "types.h"

/*
 * resource_3b9 owner at 0x02001c5c, 12 bytes: single-call trampoline,
 * forwarding `arg0` unchanged and `arg1` masked to its low 16 bits,
 * plus a fixed third argument.
 *
 * Complete owner: `push {lr}` at 0x02001c5c through `pop {r0} / bx r0`
 * at 0x02001c68-0x02001c6a; no literal pool, the next owner's prologue
 * is immediately at 0x02001c6c (this overlay's own
 * resource_3b9_c_02001c6c.c). `arg0` is never freshly loaded before the
 * call, so it is a forwarded pass-through parameter; void return.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2`
 * rule -- the second-highest call count in this mandate's sweep so far
 * (46 sites).
 *
 * Uncertainty: the callee is not identified beyond call shape.
 */

extern void Func_02004922(s32 arg0, u16 arg1, s32 arg2);

void Func_02001c5c(s32 arg0, s32 arg1)
{
    Func_02004922(arg0, (u16)arg1, 10);
}
