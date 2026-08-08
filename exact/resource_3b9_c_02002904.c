#include "types.h"

/*
 * resource_3b9 owner at 0x02002904, 96 bytes: a linear setup sequence --
 * one call with two large fixed-point-looking constants, then three
 * near-identical calls with a shared second constant (214<<1) and a
 * varying first, then a short closing run.
 *
 * Complete owner: `push {lr}` at 0x02002904 through `pop {r0} / bx r0`
 * at 0x02002958-0x0200295a, followed by the two-word literal pool
 * 0x0200295c-0x02002963; the next owner's prologue is at 0x02002964
 * (this overlay's own resource_3b9_c_02002964.c, since drafted). No incoming arguments are read before being overwritten, so
 * `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2`
 * rule.
 *
 * Uncertainty: none of the nine callees are identified beyond call
 * shape; the two large pool constants (0xcccc, 0x19999) are recorded as
 * passed values.
 */











extern void Func_020054fc(void);
extern void Func_02005526(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005632(void);
extern void Func_0200557a(s32 arg0, s32 arg1);
extern void Func_02005568(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005576(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200557c(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005670(void);
extern void Func_0200567c(void);
extern void Func_02005652(s32 arg0);
void Func_02002904(void)
{
    Func_020054fc();
    Func_02005526(0, 0x19999, 0xcccc);
    Func_02005632();
    Func_0200557a(0, 2);
    Func_02005568(0, 800, 428);
    Func_02005576(0, 700, 428);
    Func_0200557c(0, 600, 428);
    Func_02005670();
    Func_0200567c();
    Func_02005652(22);
}
