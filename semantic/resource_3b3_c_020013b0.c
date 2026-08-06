#include "types.h"

/*
 * Resource 3b3 damped-motion step at 0x020013b0 (118 bytes, 3 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020013b0 — `push {r5, r6, lr}`
 * then r8 and sl through r5,r6 — the body is straight-line with no conditional
 * branch, and the walk stops at the matching high-register unwind
 * `pop {r3, r5} / mov r8,r3 / mov sl,r5 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x0200141a-0x02001424.  So the executable extent is exactly
 * 0x020013b0-0x02001425 (118 bytes).  The popped register is r0, so the popped
 * value is the return address and the owner is `void`.
 *
 * Pool map: none — this owner loads no literal; the three divisors are `movs`
 * immediates.  The halfword at 0x02001426 is `0x0000` alignment before the
 * next prologue at 0x02001428, and the walk never reaches it as an
 * instruction.
 *
 * All three call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_3b3 13b0 1426`: 0x020013da,
 * 0x020013e6 and 0x020013f4 all reach veneer 0x02002a2c -> Func_03000380, the
 * relocated IWRAM quotient helper.  `overlay_show` prints three DIFFERENT
 * targets for them (0x2003e08, 0x2003e14, 0x2003e22), which is the
 * pc-relative bug's signature and is not used.
 *
 * THIS IS THE SAME ROUTINE AS resource_3c9_c_020003a0.c, transposed and
 * re-scheduled rather than copied: identical field map (+68/+72/+76 deltas
 * integrated into +8/+12/+16, the +48/+52 pair added into +24/+28, and the
 * pointer at +80 accumulating this object's u16 at +100 into its own +30), and
 * the two agree on every offset.  Two differences are real and neither is
 * cosmetic:
 *   * 3c9 decays only two of the three axes (22 and 20) and leaves +72 alone;
 *     this copy decays all three, with 10, 3 and 10.
 *   * 3c9 spills its copy of the velocity to the stack across the call
 *     because r4 is call-clobbered; this copy parks the other two velocities
 *     in r8 and sl instead, which is why it saves high registers at all.
 * Reading either as the other would silently move a decay onto an axis that
 * does not have one.
 */

           /* relocated IWRAM quotient helper */

extern s32 Func_02003e08();
extern s32 Func_02003e14();
extern s32 Func_02003e22();
void Func_020013b0(u8 *object)
{
    s32 vx = *(s32 *)(object + 68);
    s32 vy = *(s32 *)(object + 72);
    s32 vz = *(s32 *)(object + 76);

    *(s32 *)(object + 8) += vx;
    *(s32 *)(object + 12) += vy;
    *(s32 *)(object + 16) += vz;

    vx -= Func_02003e08(vx, 10);
    *(s32 *)(object + 68) = vx;

    vy -= Func_02003e14(vy, 3);
    *(s32 *)(object + 72) = vy;

    vz -= Func_02003e22(vz, 10);
    *(s32 *)(object + 76) = vz;

    *(s32 *)(object + 24) += *(s32 *)(object + 48);
    *(s32 *)(object + 28) += *(s32 *)(object + 52);

    {
        u8 *target = *(u8 **)(object + 80);

        *(u16 *)(target + 30) =
            (u16)(*(u16 *)(target + 30) + *(u16 *)(object + 100));
    }
}
