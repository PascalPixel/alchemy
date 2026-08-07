#include "types.h"

/*
 * Resource 3c9 damped-motion step at 0x020003a0 (94 bytes, 2 calls).
 *
 * Derived span, not an inventory row: this owner has no row in
 * out/decomp/overlays.json and no exact sibling.  It was found by sweeping the
 * two-byte gaps in metrics/gs1-en-executable.json for `push {..,lr}`
 * prologues.  Walking from the prologue at 0x020003a0 the body is
 * straight-line — two `bl`s, no conditional branch — and the walk stops at the
 * interworking return `add sp,#4 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x020003f6-0x020003fc.  So the executable extent is exactly
 * 0x020003a0-0x020003fd (94 bytes).  The popped register is r0, so the popped
 * value is the return address and the owner is `void`.
 *
 * Pool map: none — this owner loads no literal.  The halfword at 0x020003fe is
 * `0x0000` alignment before the next prologue at 0x02000400, and the walk
 * never reaches it as an instruction.
 *
 * Both call sites were resolved with
 * `bun tools/overlay_call_targets.ts resource_3c9 03a0 03fe`: 0x020003c4 and
 * 0x020003d2 both reach veneer 0x02005bec -> Func_03000380, the ARM-mode
 * helper relocated into IWRAM.  They are two sites with DIFFERENT printed
 * targets in `overlay_show` (0x2005fb2 and 0x2005fc0) and the same real
 * callee, which is the pc-relative bug's signature; the printed names are not
 * used.
 *
 * Func_03000380 is the established two-argument quotient helper (see
 * resource_371_c_0200008c.c, which calls it as `Func_03000380(level * 9,
 * 10)`), so `v -= Func_03000380(v, 22)` is a proportional decay of the stored
 * velocity by a twenty-second per frame, and the second is a twentieth.  The
 * `sub sp,#4 / str r4,[sp] / ldr r4,[sp]` around the first call is the
 * original spilling its own copy of the velocity across the call rather than a
 * second variable — r4 is not saved by this prologue, which is the documented
 * call-clobbered-r4 convention in this family, so it must be reloaded.
 *
 * The three axis deltas at +68/+72/+76 are integrated into +8/+12/+16, but
 * only two of them decay: +72 is applied and left alone.  A second pair at
 * +48/+52 is integrated into +24/+28 with no decay at all.  Do not tidy those
 * into one loop; the asymmetry is the behaviour.
 *
 * The last three lines follow the pointer at +80 and add this object's u16 at
 * +100 into the target's u16 at +30, which the neighbouring owner at
 * 0x02000400 then wraps with a 0xffff bias — so the pair is an accumulator and
 * its wrap, not two unrelated halfwords.
 */

s32 Func_03000380();           /* relocated IWRAM quotient helper */

void Func_020003a0(u8 *object)
{
    s32 vx = *(s32 *)(object + 68);
    s32 vy;
    s32 vz;

    *(s32 *)(object + 8) += vx;

    vy = *(s32 *)(object + 72);
    *(s32 *)(object + 12) += vy;

    vz = *(s32 *)(object + 76);
    *(s32 *)(object + 16) += vz;

    vx -= Func_03000380(vx, 22);
    *(s32 *)(object + 68) = vx;

    {
        s32 damped = Func_03000380(vz, 20);
        s32 t;
        s32 u;

        *(s32 *)(object + 24) += *(s32 *)(object + 48);

        t = *(s32 *)(object + 52);
        u = *(s32 *)(object + 28);
        vz -= damped;
        u += t;
        *(s32 *)(object + 76) = vz;
        *(s32 *)(object + 28) = u;
    }

    {
        u8 *target = *(u8 **)(object + 80);

        *(u16 *)(target + 30) =
            (u16)(*(u16 *)(target + 30) + *(u16 *)(object + 100));
    }
}
