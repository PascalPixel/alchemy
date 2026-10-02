/* DRAFT of Graphics_PrepareTransfer: builds a look-at matrix. The forward
   axis is target - eye, normalised; the side axis is perpendicular to it in
   the ground plane; the third is their cross product. Each row ends with
   minus the eye's dot product with its axis. The dot product runs from a
   28-byte routine copied to the stack.
   Not exact: 500 of 500 bytes. Remaining: after the third product the ROM
   negates dx before storing dz, and in the tail it keeps the zero in r0 and
   reloads uz through r1, where this build uses r3 and r0. */
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
    s32 ex;
    Dot3Fn dot;
    s32 ey;
    s32 uz;
    s32 vx, vy, vz;
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
    out[0] = ux;
    out[3] = 0;
    out[6] = uz;
    out[9] = -dot(ex, ux, ez, uz, 0, 0);
    out[4] = vy;
    out[7] = vz;
    out[1] = vx;
    out[10] = -dot(ex, vx, ey, vy, ez, vz);
    out[2] = dx;
    out[5] = dy;
    out[8] = dz;
    out[11] = -dot(ex, dx, ey, dy, ez, dz);
}
