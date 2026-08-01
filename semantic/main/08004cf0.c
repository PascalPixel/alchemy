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
typedef signed int s32;

typedef void (*Resident_030002C0)(void *target, s32 arg1, s32 arg2);


void Func_08004cf0(s32 *input) {
    s32 values[12];
    values[0] = 0x10000;
    values[1] = 0;
    values[2] = 0;
    values[3] = 0;
    values[4] = 0x10000;
    values[5] = 0;
    values[6] = 0;
    values[7] = 0;
    values[8] = 0x10000;
    values[9] = 0;
    values[10] = 0;
    values[11] = 0;
    values[0] = input[0];
    values[4] = input[1];
    values[8] = input[2];
    ((Resident_030002C0)0x030002c0)(values, 0x10000, 0);
}
