#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x0200413c (92 bytes incl. pool,
 * 1 call).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {lr}` at 0x0200413c, epilogue `pop {r0} / bx r0` at
 * 0x0200418e-0x02004190. The one-word literal pool at 0x02004194
 * (0x00001e43, same `Value_XXXXXXXX` low-address family as elsewhere in
 * this overlay) is included per the usual pool rule, immediately
 * followed by the next owner's push {r5,lr} at 0x02004198, already this
 * overlay's row `0x02004198 | 1 call`, so the span is
 * 0x0200413c-0x02004198, 92 bytes.
 *
 * A flat setter sequence, no branches. Two distinct call sites happen to
 * target the same callee (`Func_0200a752`) with different arities.
 *
 * Per-site call symbols (law 6): every callee below is named at the
 * bl-site-symbols-decoded address for its own site, not the raw
 * main-image address the earlier draft used.
 */

extern u8 Value_00001e43;

void Func_0200a608(void);
void Func_0200a752();
void Func_0200a578();
void Func_02008a4a();
void Func_0200a6ea();
void Func_0200a718();
void Func_020089e2();
void Func_02008a74();

void Func_0200413c(void)
{
    Func_0200a608();
    Func_0200a752(-1, -1, -1, 0);
    Func_0200a578(1);
    Func_02008a4a(15, 1, 1);
    Func_0200a6ea(8, 1);
    Func_0200a718((s32)&Value_00001e43);
    Func_020089e2(8);
    Func_0200a752(8, 0xd000, 0x28);
    Func_02008a74(9, 15, 0);
}
