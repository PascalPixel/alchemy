/* 2026-09-24: hand-written, 207 differing halfwords (508 of 500 bytes).
   Look-at matrix: the dot product runs from a 28-byte routine DMA-copied to
   the stack (called through r4), the divide through fp (0x030003f0) and the
   square root through 0x030001d8. The reference spills dx/dy/dz to
   sp+28/24/20, ux to sp+16 and keeps out in r9, the divide in fp; here the
   allocation differs throughout. */
#include "TYPES.H"
#include "DMA.H"
#include "IWRAM_CALL.H"

typedef s32 (*Dot3Fn)(s32 ax, s32 bx, s32 ay, s32 by, s32 az, s32 bz);
typedef s32 (*SqrtFn)(s32 value);
typedef u32 (*DivFn)(u32 numerator, u32 denominator);

s32 FixedSqrt(s32 value);

void Graphics_PrepareTransfer(s32 *eye, s32 *target, s32 *out)
{
    s32 dx, dy, dz;
    s32 ux;
    Dot3Fn dot;
    s32 ey;
    s32 uz;
    s32 vx, vy, vz;
    s32 scale;
    s32 ex, ez;
    u32 code[7];

    dot = (Dot3Fn)code;
    Dma_Set((void *)0x08007994, code, 0x84000007, (volatile u32 *)0x040000d4);
    dx = target[0] - eye[0];
    dy = target[1] - eye[1];
    dz = target[2] - eye[2];
    scale = -(((DivFn)0x030003f0)(0x80000000, ((SqrtFn)0x030001d8)(dot(dx >> 8, dx >> 8, dy >> 8, dy >> 8, dz >> 8, dz >> 8))) >> 15);
    dx = Iwram_MulQ16(dx, scale);
    dy = Iwram_MulQ16(dy, scale);
    dz = Iwram_MulQ16(dz, scale);
    uz = -dx;
    if (0x10000 - Iwram_MulQ16(dy, dy) > 0)
        scale = ((DivFn)0x030003f0)(0x80000000, FixedSqrt(0x10000 - Iwram_MulQ16(dy, dy))) << 1;
    ux = Iwram_MulQ16(dz, scale);
    uz = Iwram_MulQ16(uz, scale);
    vx = Iwram_MulQ16(dy, uz);
    vy = Iwram_MulQ16(dz, ux) - Iwram_MulQ16(dx, uz);
    vz = -Iwram_MulQ16(dy, ux);
    scale = ((DivFn)0x030003f0)(0x80000000, FixedSqrt(dot(vx, vx, vy, vy, vz, vz))) << 1;
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
    out[1] = vx;
    out[4] = vy;
    out[7] = vz;
    out[10] = -dot(ex, vx, ey, vy, ez, vz);
    out[2] = dx;
    out[5] = dy;
    out[8] = dz;
    out[11] = -dot(ex, dx, ey, dy, ez, dz);
}

