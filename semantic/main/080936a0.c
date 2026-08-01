/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * 0x0300013c is the relocated ARM ratio helper, established in
 * semantic/main/080e15e8.c as (numerator, denominator) -- TWO arguments.
 * This draft passed three, inventing a trailing zero. Checked against the
 * ROM rather than trusted: at the `bl` at 0x080936c8, r0 takes arg0, r1
 * takes 0x10000 and r3 the callee, and NOTHING writes r2. Two it is, which
 * independently corroborates 080e15e8's reading at a second call site.
 */
#include "types.h"

typedef s32 (*ArmRatio)(s32 numerator, s32 denominator);

struct State {
    u8 filler0[848];
    u32 previous;
    u32 current;
    u16 kind;
    u16 flags;
};

s32 Func_080048f4(s32 arg0, s32 arg1);
void Func_080041d8(const void *arg0, s32 arg1);

void Func_080936a0(s32 arg0, s32 arg1)
{
    struct State *state = *(struct State **)0x03001e70;
    s32 handle;
    s32 result;

    handle = Func_080048f4(27, 0xccc);
    if (*(s16 *)(handle + 414) != 3)
        return;
    result = ((ArmRatio)0x0300013c)(arg0, 0x10000);
    state->previous = state->current;
    state->current = result;
    state->kind = arg1;
    state->flags = 0;
    Func_080041d8((const void *)0x080935d5, 0xc94);
}
