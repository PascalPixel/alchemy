/* 2026-10-02: complete English extent scores 0, including the literal pool.
   Baseline 505: 4 register, 2 operand, 4 reordered, 1 inserted/1 deleted.
   Natural source's remaining negation/spill and zero/axis-copy order did not
   follow its statement order. Used-value trials: early uz output barrier
   1590; add r8 binding 770; rest output barrier 1380; scoped r0 zero 300;
   add vx/r10 290; pin whole destination/r9 4109 (rejected global lifetime);
   ex/r11 with natural vx 240; ex/r11 + vx/r10 180; rest barrier then 980;
   add used uz/r8 60. Scoped row/r4 read-write 604 and input-only 649 both
   altered spills, rejected. Moving zero's source lifetime after out[0]
   stayed 60. Making uz depend on zero 210 altered eye loads, rejected.
   Retained r1 clobber on the consumed zero constraint: 0. All storage and
   emitted instructions carry actual function work. Adoption still requires
   every edition's complete linked ROM comparison. */
/* DRAFT of Graphics_PrepareTransfer: builds a look-at matrix. The forward
   axis is target - eye, normalised; the side axis is perpendicular to it in
   the ground plane; the third is their cross product. Each row ends with
   minus the eye's dot product with its axis. The dot product runs from a
   28-byte routine copied to the stack.
   Exact draft: complete 500-byte owner including alignment and literal pool. */
#include "TYPES.H"
#include "DMA.H"
#include "IWRAM_CALL.H"

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);

extern const u32 RelocatedQ16DotProduct[];
s32 FixedSqrt(s32 value);

void Graphics_PrepareTransfer(s32 *eye, s32 *target, s32 *out)
{
    u32 code[7];
    s32 dx, dy, dz;
    s32 ux;
    /* FAKEMATCH: the used eye x component remains in r11 through the row dot calls. */
    register s32 ex asm("r11");
    Dot3Fn dot;
    s32 ey;
    /* FAKEMATCH: the used perpendicular axis stays in r8 through the cross product. */
    register s32 uz asm("r8");
    /* FAKEMATCH: the used third-row x component stays in r10 through normalization. */
    register s32 vx asm("r10");
    s32 vy, vz;
    s32 scale;
    s32 ez;
    s32 rest;
    u32 one = 0x80000000;
    u32 (*divide)(u32 dividend, u32 divisor);

    dot = (Dot3Fn)code;
    Dma_Set(RelocatedQ16DotProduct, code, 0x84000007, (volatile u32 *)0x040000d4);
    dx = target[0] - eye[0];
    dy = target[1] - eye[1];
    dz = target[2] - eye[2];
    rest = Iwram_Sqrt(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8));
    divide = Iwram_UnsignedDivide;
    scale = divide(one, rest) >> 15;
    scale = -scale;
    dx = Iwram_MulQ16(dx, scale);
    dy = Iwram_MulQ16(dy, scale);
    rest = Iwram_MulQ16(dz, scale);
    uz = -dx;
    /* FAKEMATCH: keep the used negated axis before spilling the third product. */
    asm("" : "+r"(rest) : "r"(uz));
    dz = rest;
    rest = Iwram_MulQ16(dy, dy);
    scale = 0x10000;
    if (scale - rest > 0)
        scale = divide(one, FixedSqrt(scale - rest)) << 1;
    ux = Iwram_MulQ16(dz, scale);
    uz = Iwram_MulQ16(uz, scale);
    vx = Iwram_MulQ16(dy, uz);
    vy = Iwram_MulQ16(dz, ux) - Iwram_MulQ16(dx, uz);
    vz = -Iwram_MulQ16(dy, ux);
    scale = divide(one, FixedSqrt(dot(vx, vx, vy, vy, vz, vz))) << 1;
    vx = Iwram_MulQ16(vx, scale);
    vy = Iwram_MulQ16(vy, scale);
    vz = Iwram_MulQ16(vz, scale);
    ex = eye[0];
    ey = eye[1];
    ez = eye[2];
    {
        /* FAKEMATCH: the used zero argument remains in r0 beside the first row stores. */
        register s32 zero asm("r0");
        out[0] = ux;
        zero = 0;
        /* FAKEMATCH: keep the used zero in r0 and delay the used axis copy into r1. */
        asm("" : "+r"(zero) : : "r1");
        out[3] = zero;
        out[6] = uz;
        out[9] = -dot(ex, ux, ez, uz, zero, zero);
    }
    out[4] = vy;
    out[7] = vz;
    out[1] = vx;
    out[10] = -dot(ex, vx, ey, vy, ez, vz);
    out[2] = dx;
    out[5] = dy;
    out[8] = dz;
    out[11] = -dot(ex, dx, ey, dy, ez, dz);
}
