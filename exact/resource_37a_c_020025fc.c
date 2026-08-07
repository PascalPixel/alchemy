#include "types.h"

/*
 * resource_37a owner at 0x020025fc, 24 bytes: a two-argument shim --
 * it hands its FIRST argument to one main-image routine with a zero, then
 * its SECOND to another, and returns nothing.
 *
 * Complete owner: `push {r5, lr}` at 0x020025fc through the single
 * epilogue `pop {r5} / pop {r0} / bx r0` at 0x0200260c-0x02002610,
 * no literal pool. Next owner at 0x02002614. Measured to the epilogue.
 *
 * BOTH arguments are real. r1 is saved into r5 at 0x020025fe BEFORE r1
 * is overwritten with 0 for the first call, and r0 reaches the first call
 * untouched -- so the row reads both incoming registers before writing
 * either. That save is the only reason r5 is pushed at all.
 *
 * Call sites resolved with `bun tools/lib/overlay_call_targets.ts resource_37a
 * 25fc 2614`'s `+2` rule: `sites=2 veneer=2`, and two are transcribed.
 *
 * NOT PUBLISHED -- it is the one owner in this overlay's residue that is
 * bl-reached instead. Every 4-aligned word was scanned for 0x0200a5fc
 * with and without the Thumb bit and there are none, which matches sweep A
 * having found it and sweep B not.
 *
 * It is also the whole of this overlay's single sweep-D gap
 * (`CODE-SUSPECT 0x25fc-0x2613 24B after 0x25b0`), which is why sweep D was
 * run FIRST: the unkeyed sweep and the keyed one agreed on this address, and
 * agreement between instruments that work differently is worth more than
 * either alone.
 *
 * Uncertainty: neither Func_0808a180 nor Func_0808a010 is established here,
 * so both are declared old-style and the zero passed to the first is
 * transcribed rather than interpreted.
 */




extern void Func_02005068();
extern void Func_02004fd6();
void Func_020025fc(s32 arg0, s32 arg1)
{
    Func_02005068(arg0, 0);
    Func_02004fd6(arg1);
}
