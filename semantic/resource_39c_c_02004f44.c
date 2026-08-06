#include "types.h"

/*
 * resource_39c owner at 0x02004f44, 76 bytes: fire a three-call setup
 * sequence through this overlay's import table, then set bit 3 of a byte
 * inside a struct reached through the global pointer at 0x03001f30.
 *
 * Complete owner: `push {r5, lr}` at 0x02004f44 through `pop {r5} / pop {r0}
 * / bx r0` at 0x2004f82-0x2004f86, followed by the two-word literal pool
 * 0x02004f88-0x02004f8f; the next owner's prologue is at 0x02004f90.  No
 * incoming arguments are read before being overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only by
 * `bl`, resolved with `bun tools/overlay_call_targets.ts`'s `+2` rule.
 *
 * Func_0808a0f0's fourth argument is the address of `Data_03001f30` itself
 * (the same cell this owner later dereferences for the byte-flag store),
 * not a value read through it -- the address is passed on, not read, at
 * that call site.
 *
 * Uncertainty: Func_0808a448 and Func_0808a438 are called with no register
 * freshly loaded for the call (each reuses whatever a prior, unrelated
 * call's argument left in r0), so they are declared old-style with no
 * argument list rather than credited with a phantom parameter.  None of the
 * six callees are identified beyond their call shape; the two shifted
 * constants passed to Func_0808a0f0 (0x3480000, 0x2580000) are recorded as
 * plain values, not decoded further.
 */

extern u8 *Data_03001f30;








extern void Func_0200aa7a(s32 arg0, s32 arg1, s32 arg2, void *arg3);
extern void Func_0200ab8a(s32 arg0, s32 arg1);
extern void Func_0200ab9a(s32 arg0, s32 arg1);
extern void Func_0200abba();
extern void Func_0200aba0(s32 arg0);
extern void Func_0200abbc();
void Func_02004f44(void)
{
    u8 *target;

    Func_0200aa7a(11, 0x3480000, 0x2580000, &Data_03001f30);
    Func_0200ab8a(93, 1);
    Func_0200ab9a(3, 11);

    target = Data_03001f30 + 0x71c;
    *target = *target | 8;

    Func_0200abba();
    Func_0200aba0(1);
    Func_0200abbc();
}
