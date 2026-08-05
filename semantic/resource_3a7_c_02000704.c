/*
 * resource_3a7 scene-entity flag sweep at 0x02000704, 48 bytes.
 *
 * Complete owner: `push {r5, r6, r7, lr} / mov r7, r8 / push {r7}` at
 * 0x02000704 through the matching `pop {r3} / mov r8, r3 / pop {r5, r6, r7} /
 * pop {r0} / bx r0` at 0x0200072a.  The return address is popped into r0, so
 * the owner is `void`.  No literal pool inside the span.
 *
 * This is the OR-variant of the byte-exact sibling
 * assets/code/resource_3a7_c_02000734.c, which sits immediately after it and is
 * the same 6-iteration loop over the same selectors 16..21:
 *
 *     s32 index = 16, flag = 1, remaining = 5;
 *     do { entry = Func_02001870(index); remaining--; entry[35] = flag;
 *          index++; } while (remaining >= 0);
 *
 * Here the store is a read-modify-write `entry[35] |= 2` instead of an
 * assignment, and the loop is otherwise halfword-for-halfword identical.  The
 * exact sibling therefore settles the loop bounds, the +35 field and the
 * import (its printed `Func_02001870` resolves to Func_0808a080), leaving only
 * the OR to read from assembly.
 *
 * The six selectors 16..21 are exactly the six the presentation family at
 * 0x0200050c..0x02000690 addresses one at a time, and byte 35 is the field each
 * of those owners sets to 1 - so this is the batch form of the same field.
 *
 * Call targets resolved with `bun tools/overlay_call_targets.ts resource_3a7
 * 0704`.  One site, matching the inventory's calls=1:
 *   Func_0808a080 x1 <- veneer 0x02001130
 *
 * Uncertainty, recorded rather than "fixed": r8 is saved by the prologue,
 * loaded with 0 at 0x02000712 and never read again before being restored.  The
 * bytes are not in doubt; the dead assignment is simply not modelled below.
 */

/* Old-style declaration: overlay imports vary their argument count between
 * call sites in this overlay. */
#include "types.h"

u8 *Func_0808a080();    /* scene-entity record by selector */

void Func_02000704(void)
{
    s32 index = 16;
    s32 remaining = 5;

    do {
        u8 *entry = Func_0808a080(index);

        remaining--;
        index++;
        entry[35] |= 2;
    } while (remaining >= 0);
}
