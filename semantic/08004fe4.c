#include "types.h"

/*
 * From `self` toward `other`: computes the normalized direction, a second
 * vector orthogonal to it (built from the direction's horizontal-plane
 * magnitude), and three "plane constants" -- `self` dotted against each of
 * the direction, the orthogonal vector, and a third vector derived from
 * both -- writing all of it into a 12-word `out` block. 500 bytes, no
 * branches but one (the horizontal-plane fallback below).
 *
 * Full body verified against `arm-none-eabi-objdump -d out/full/asm/
 * 08004fe4.elf` and cross-checked line-by-line against `asm/08004fe4.s`;
 * every register in the trace below is accounted for, not inferred.
 *
 * HANDOVER stale-list correction (already recorded once in the reconstruction's
 * park note): 08004fe4 is on HANDOVER's 14-stem "ARM-only" list, but
 * `asm/08004fe4.s` has `.thumb`, no ARM directive, and never carries the
 * Thumb-only-compiler header. Genuine convertible Thumb, same finding as
 * 08002dd8 (commit ab80735a).
 *
 * THE STACK-RELOCATED HELPER. The owner DMAs 7 words (28 bytes, one-shot,
 * enable + 32-bit + increment/increment, REG_DMA3CNT = 0x84000007) from ROM
 * address 0x08007994 onto its OWN stack frame, then calls the copy through
 * the ordinary call-via-r4 thunk for the rest of the function. This is a
 * variant of the IWRAM-relocation pattern this project already knows
 * (0x03000118 multiply, 0x030001d8 sqrt, 0x030003f0 divide, all copied once
 * to a fixed IWRAM address at startup) -- except this helper is relocated
 * fresh onto the CALLER's stack on every call instead of to a fixed
 * address, presumably because it is needed by only a few call sites and
 * is not worth a permanent IWRAM slot. `asm/08007994.s` is already
 * reconstructed and named -- its own comment (Japanese) reads "a signed
 * 16.16 fixed-point three-term dot product transferred to the stack for
 * execution; the caller passes six components, upper and lower combined
 * without rounding" -- and its body is genuine ARM (`smull`/`smlal`, `.arm`
 * directive), so the plain even stack address is callable through `bx`
 * exactly like the other IWRAM helpers, no Thumb bit needed. Declared here
 * as `QDot3`: `dot3(ax,bx,ay,by,az,bz) = ax*bx + ay*by + az*bz`, called
 * with the same pair duplicated in all three slots wherever a squared
 * magnitude is wanted (matching the resource_3b3/resource_3c9 overlay
 * distance helpers' `dx*dx+dy*dy+dz*dz` shape, just via a shared routine
 * instead of inline multiplies).
 *
 * THE OTHER THREE HELPERS, all already named elsewhere in this tree:
 *   - 0x030001d8, reached via call-via-r3: the IWRAM-relocated square root
 *     (resource_3b3_c_02000314.c, resource_3b7_c_02000e5c.c).
 *   - 0x030003f0, reached via call-via-fp: the IWRAM-relocated divide,
 *     `divide(numerator, denominator)` (semantic/main/08005268.c's
 *     `Divide_08005268` typedef, copied here).
 *   - 0x03000118, reached inline via `mov ip,pc; bx r5`: the IWRAM
 *     fixed-point multiply (same file's `Multiply_08005268` typedef).
 *   - `Func_080045d4(sum_of_squares)`: a DIFFERENT, main-image square root
 *     (not IWRAM-relocated), already declared with exactly this signature
 *     in semantic/main/0800d14c.c and semantic/main/0800daf0.c. Called
 *     twice here on values already known to be small (a horizontal-plane
 *     magnitude, and a second cross-vector's squared magnitude) rather
 *     than on the raw distance, which is presumably why the cheaper
 *     main-image routine is used instead of the IWRAM one.
 *
 * THE GEOMETRY. `direction` (`ndx,ndy,ndz`) is `other - self`, scaled by
 * `-1/distance` (distance from the IWRAM sqrt of the squared delta, scale
 * from the IWRAM divide of 0x80000000 by that distance, `>>15`, then
 * negated) -- so `direction` points from `other` back toward `self`, not
 * the way its name might suggest; the exact fixed-point scale of the
 * result was not independently re-derived. `horizontalMag2 = 0x10000 -
 * ndy*ndy` is `1 - ndy^2` in Q16, i.e. a plane-projected magnitude if
 * `direction` is itself a Q16 unit vector, which the two sqrt/divide/
 * fallback branches (rescue value `scale1` when the plane magnitude would
 * be non-positive) are consistent with but do not prove. From there the
 * owner builds a second vector (`basisX,basisY,basisZ`) orthogonal-looking
 * to `direction`, normalizing it the same way, and finally dots `self`'s
 * raw position against `direction`, against the basis vector, and against
 * a third vector built from `rz`/`rxNeg` -- three "plane constant" values,
 * each negated on the way into `out`. Read together this is consistent
 * with building an orientation basis plus per-plane distances (billboard
 * facing, a clip plane, or similar), but that reading is not confirmed
 * against any caller.
 *
 * `out`'s layout was checked against `struct MotionObject`
 * (include/motion_object.h) since offsets 36/40/44 coincide with its
 * velocity_x/y/z -- but offset 12 here is an unconditional 0, where
 * MotionObject's own offset 12 is `y` (a live field), which argues against
 * `out` being read back as a MotionObject rather than a bespoke 12-word
 * scratch block. Left as a flat block rather than forcing a struct.
 *
 * Uncertainty: `self`/`other` are read only at word offsets 0/4/8, so
 * declared as plain 3-word vectors, not tied to any named struct; `out`'s
 * word 3 (offset 12, always 0) and the overall geometric purpose of the
 * whole computation are inferred, not confirmed.
 */

typedef s32 (*QDot3_08004fe4)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);
typedef s32 (*IsqrtIwram_08004fe4)(s32 value);
typedef s32 (*Divide_08004fe4)(s32 numerator, s32 denominator);
typedef s32 (*Multiply_08004fe4)(s32 a, s32 b);

extern s32 Func_080045d4(s32 sum_of_squares); /* main-image fixed-point sqrt */

void Func_08004fe4(const s32 *self, const s32 *other, s32 *out)
{
    s32 buffer[7];
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    QDot3_08004fe4 dot3;
    IsqrtIwram_08004fe4 isqrtIwram = (IsqrtIwram_08004fe4)0x030001d8;
    Divide_08004fe4 divide = (Divide_08004fe4)0x030003f0;
    Multiply_08004fe4 multiply = (Multiply_08004fe4)0x03000118;

    s32 dx, dy, dz;
    s32 dist, scale1, scale2, scale3;
    s32 ndx, ndy, ndz;
    s32 negNdx, ndySq, horizontalMag2;
    s32 rz, rxNeg;
    s32 c1, termA, c2, c3Neg;
    s32 basisX, basisY, basisZ;
    s32 planeA, planeB, planeC;

    dma3[0] = 0x08007994;    /* SAD: RelocatedQ16DotProduct_08007994's bytes */
    dma3[1] = (u32)buffer;   /* DAD: this frame's local buffer */
    dma3[2] = 0x84000007;    /* CNT: enable, 32-bit, 7 words, one-shot */
    dot3 = (QDot3_08004fe4)(u32)buffer;

    dx = other[0] - self[0];
    dy = other[1] - self[1];
    dz = other[2] - self[2];

    dist = isqrtIwram(dot3(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
    scale1 = -(divide(0x80000000, dist) >> 15);

    ndx = multiply(dx, scale1);
    ndy = multiply(dy, scale1);
    ndz = multiply(dz, scale1);
    negNdx = -ndx;

    ndySq = multiply(ndy, ndy);
    horizontalMag2 = 0x10000 - ndySq;
    if (horizontalMag2 > 0) {
        scale2 = divide(0x80000000, Func_080045d4(horizontalMag2)) << 1;
    } else {
        scale2 = scale1;
    }

    rz = multiply(ndz, scale2);
    rxNeg = multiply(negNdx, scale2);

    c1 = multiply(ndy, rxNeg);
    termA = multiply(ndz, rz);
    c2 = termA - multiply(ndx, rxNeg);
    c3Neg = -multiply(ndy, rz);

    scale3 = divide(0x80000000, Func_080045d4(dot3(c1, c1, c2, c2, c3Neg, c3Neg))) << 1;

    basisX = multiply(c1, scale3);
    basisY = multiply(c2, scale3);
    basisZ = multiply(c3Neg, scale3);

    out[0] = rz;
    out[1] = basisX;
    out[2] = ndx;
    out[3] = 0;
    out[4] = basisY;
    out[5] = ndy;
    out[6] = rxNeg;
    out[7] = basisZ;
    out[8] = ndz;

    planeA = -dot3(self[0], rz, self[2], rxNeg, 0, 0);
    out[9] = planeA;

    planeB = -dot3(self[0], basisX, self[1], basisY, self[2], basisZ);
    out[10] = planeB;

    planeC = -dot3(self[0], ndx, self[1], ndy, self[2], ndz);
    out[11] = planeC;
}
