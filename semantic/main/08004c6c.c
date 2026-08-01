/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
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
typedef unsigned int u32;
typedef signed int s32;

typedef void (*Resident_030002C0)(void *target, s32 arg1, s32 arg2);

struct Entry_08004c6c {
    u32 value;
    u32 zero4;
    u32 zero8;
    u32 zeroC;
};

s32 Func_08002322(s32);
s32 Func_0800231c(s32);

void Func_08004c6c(s32 angle)
{
    struct Entry_08004c6c entries[3];
    s32 sine;
    s32 cosine;

    sine = Func_08002322(angle);
    cosine = Func_0800231c(angle);
    entries[0].value = 0x10000;
    entries[0].zero4 = 0;
    entries[0].zero8 = 0;
    entries[0].zeroC = 0;
    entries[1].value = 0x10000;
    entries[1].zero4 = 0;
    entries[1].zero8 = 0;
    entries[1].zeroC = 0;
    entries[2].value = 0x10000;
    entries[2].zero4 = 0;
    entries[2].zero8 = 0;
    entries[2].zeroC = 0;
    entries[0].zero4 = sine;
    entries[0].value = cosine;
    entries[1].value = cosine;
    entries[0].zeroC = -sine;
    ((Resident_030002C0)0x030002c0)(entries, 0x10000, 0);
}
