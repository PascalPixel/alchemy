#include "types.h"

/*
 * Resource 3ad, owner at 0x02000460 (72 bytes, 7 call sites).
 *
 * Complete owner: `push {lr}` at 0x02000460, `pop {r0} / bx r0` at 0x02000496.
 * r0 is the popped register, so it holds the return address: the owner is
 * `void`.  A three-word literal pool sits at 0x0200049c-0x020004a7, past the
 * return and never reached as code (0x202, 0x1528, 0x201).
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`:
 *   0x02000466 -> veneer 0x02001b04 -> Func_080770c0
 *   0x02000470 -> veneer 0x02001b04 -> Func_080770c0
 *   0x0200047c -> veneer 0x02001afc -> Func_08015040
 *   0x02000482 -> veneer 0x02001c2c -> Func_080f9010
 *   0x02000486 -> 0x02000384 (this overlay's own prologue)
 *   0x0200048c -> veneer 0x02001b0c -> Func_080770c8
 *   0x02000492 -> veneer 0x02001b14 -> Func_080770d0
 * Per-target multiset: Func_080770c0 x2 and one each of Func_08015040,
 * Func_080f9010, Func_02000384, Func_080770c8, Func_080770d0 - 7 sites, 7 C
 * call expressions.
 *
 * Mirror of 0x0200033c: byte-identical apart from the exchanged story-flag
 * constants (0x202 here where that owner uses 0x201) and the body call target.
 * Note the guard order differs too - this owner tests 0x200 first and 0x202
 * second, where 0x0200033c tests 0x201 first and 0x200 second.  Both orders are
 * as encoded; only the immediate-vs-pool spelling of 0x200 gives it away.
 */

/* Old-style declarations: overlay import arities vary per call site. */
    /* test a story flag (used in a condition) */
   /* set a story flag */
   /* clear a story flag */


   /* this overlay, the scene body for the 0x202 state */

extern s32 Func_02001f6c();
extern s32 Func_02001f76();
extern void Func_02001f7a();
extern void Func_020020b0();
extern void Func_0200080c();
extern void Func_02001f9a();
extern void Func_02001fa8();
void Func_02000460(void)
{
    if (Func_02001f6c(0x200) != 0) {
        return;
    }
    if (Func_02001f76(0x202) != 0) {
        return;
    }

    Func_02001f7a(0x1528, 1);
    Func_020020b0(0x9d);
    Func_0200080c();

    Func_02001f9a(0x202);
    Func_02001fa8(0x201);
}
