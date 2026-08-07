#include "types.h"

/*
 * Resource 3ad, owner at 0x0200033c (72 bytes, 7 call sites).
 *
 * Complete owner: `push {lr}` at 0x0200033c, `pop {r0} / bx r0` at 0x02000372.
 * r0 is the popped register, so it holds the return address: the owner is
 * `void`.  A three-word literal pool sits at 0x02000378-0x02000383, past the
 * return and never reached as code (0x201, 0x1528, 0x202).
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`:
 *   0x02000340 -> veneer 0x02001b04 -> Func_080770c0
 *   0x0200034c -> veneer 0x02001b04 -> Func_080770c0
 *   0x02000358 -> veneer 0x02001afc -> Func_08015040
 *   0x0200035e -> veneer 0x02001c2c -> Func_080f9010
 *   0x02000362 -> 0x02000210 (this overlay's own prologue)
 *   0x02000368 -> veneer 0x02001b0c -> Func_080770c8
 *   0x0200036e -> veneer 0x02001b14 -> Func_080770d0
 * Per-target multiset: Func_080770c0 x2 and one each of Func_08015040,
 * Func_080f9010, Func_02000210, Func_080770c8, Func_080770d0 - 7 sites, 7 C
 * call expressions.
 *
 * This owner is one of a mirrored pair with 0x02000460, which is byte-identical
 * except that the two story-flag constants 0x201/0x202 are exchanged and its
 * body call goes to 0x02000384 instead of 0x02000210.  The two flags are set
 * and cleared against each other, so they read as the two states of one
 * two-valued scene selector.
 */

/* Old-style declarations: overlay import arities vary per call site. */
s32 Func_080770c0();    /* test a story flag (used in a condition) */
void Func_080770c8();   /* set a story flag */
void Func_080770d0();   /* clear a story flag */
void Func_08015040();
void Func_080f9010();
void Func_02000210();   /* this overlay, the scene body for the 0x201 state */

void Func_0200033c(void)
{
    /* 0x200 is built as `movs r0,#128 / lsls r0,#2`; 0x201/0x202 are pooled. */
    if (Func_080770c0(0x201) != 0) {
        return;
    }
    if (Func_080770c0(0x200) != 0) {
        return;
    }

    Func_08015040(0x1528, 1);
    Func_080f9010(0x9d);
    Func_02000210();

    Func_080770c8(0x201);
    Func_080770d0(0x202);
}
