/*
 * Correctness fix, veneer audit (2026-08-01).
 * 0x080072e4 begins the GCC `__call_via_rN` veneer bank -- fifteen four-byte
 * `bx rN; nop` entries, r0..lr, ending at 0x08007320 -- so a `bl` into that
 * range is an indirect call through the named register, not a call to a
 * function at the branch target.  Resolved with tools/veneer_resolve.ts.
 *
 * UNCERTAINTY: what the routine at 0x030002c0 DOES is not established.
 * Nothing in the tree names it.  All five callers pass the same shape --
 * a pointer, 0x10000, and 0.  The type below records that and no more; it
 * is not a claim about the routine's job.
 */
#include "types.h"

typedef void (*Resident_030002C0)(void *target, s32 arg1, s32 arg2);

struct MatrixRow_08004c1c {
    s32 first;
    s32 second;
    s32 third;
    s32 fourth;
};

struct Matrix_08004c1c {
    struct MatrixRow_08004c1c rows[3];
};

s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);

void Func_08004c1c(s32 angle)
{
    struct Matrix_08004c1c matrix;
    struct Matrix_08004c1c *destination;
    s32 sine = Func_08002322(angle);
    s32 cosine;

    destination = &matrix;
    cosine = Func_0800231c(angle);
    destination->rows[0].first = 0x10000;
    destination->rows[0].second = 0;
    destination->rows[0].third = 0;
    destination->rows[0].fourth = 0;
    destination->rows[1].first = 0x10000;
    destination->rows[1].second = 0;
    destination->rows[1].third = 0;
    destination->rows[1].fourth = 0;
    destination->rows[2].first = 0x10000;
    destination->rows[2].second = 0;
    destination->rows[2].third = 0;
    destination->rows[2].fourth = 0;

    destination->rows[0].third = -sine;
    destination->rows[0].first = cosine;
    destination->rows[1].third = sine;
    destination->rows[2].first = cosine;

    ((Resident_030002C0)0x030002c0)(destination, 0x10000, 0);
}
