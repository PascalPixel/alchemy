/* 2026-09-29 alchemy permute: score 360 to 250 on the permuter's scorer (0
   is exact); remaining 24 register-only, 2 reordered. Kept rewrites: 2x
   reorder independent statements, 2x introduce a temporary, 1x change loop
   form. FAKEMATCH: the permuter's temporaries, register hints and swapped
   operand orders below only steer allocation and scheduling; no programmer
   would write them, so they stay tagged until a natural spelling replaces
   them. */
/* Draft, not exact (2026-09-28): 420 of 420 bytes, 38 differing halfwords.
 * Draws a line into the 8bpp tiled canvas (16 tiles wide) at *0x03001EF0
 * with an 8.8 fraction, clamping y to 0..127 after the deltas are taken;
 * a pixel is only written where it raises the colour.
 * Writing the slope as a C division (the __divsi3 libcall) instead of a
 * Math_Div call fixes the frame, the extent and the allocation of x0 r8,
 * x1 r4 (saved around the shallow divide), dx r7, dy sl, frac r9, buf fp.
 * Remaining: in the steep loop the counter y takes r1 and x r0 (the ROM has
 * y r0, x r1, as the shallow loop already has x r0, y r1); the steep swap
 * computes dx before the y exchange; the shallow join value y1 - y0 sits in
 * r2 instead of r1; the fp store of buf and one pool register (r2 vs r6)
 * are scheduled differently. Block-local loop variables, shared major and
 * minor variables, loop-initialiser order, swap order and << 8 versus * 256
 * did not move the counts. */
#include "TYPES.H"

void Func_080cde90(u32 x0, s32 y0, u32 x1, s32 y1, s32 color)
{
    u8 *buf;
    s32 dx;
    s32 dy;
    s32 frac;
    s32 step;
    u32 x;
    u32 y;
    s32 t;
    u32 off;

    dx = x1 - x0;
    dy = y1 - y0;
    frac = 0x80;
    buf = *(u8 **)0x03001ef0;
    if (y0 < 0)
        y0 = 0;
    if (y0 > 127)
        y0 = 127;
    if (y1 < 0)
        y1 = 0;
    if (y1 > 127)
        y1 = 127;
    if ((dx < 0 ? -dx : dx) < (dy < 0 ? -dy : dy)) {
        s32 tmp;
        if (dy < 0) {
            t = x0;
            x0 = x1;
            x1 = t;
            t = y0;
            y0 = y1;
            dx = x1 - x0;
            y1 = t;
        }
        tmp = y1 - y0;
        step = ((s32)(x1 - x0) < 0 ? (s32)(x0 - x1) : (s32)(x1 - x0)) * 256 / (tmp < 0 ? y0 - y1 : y1 - y0);
        x = x0;
        for (y = y0; y != y1; y++) {
            off = ((((y >> 3) << 4) + (x >> 3)) << 3) + (y & 7);
            off = (off << 3) + (x & 7);
            if (buf[off] < color)
                buf[off] = color;
            frac += step;
            if (frac & 0x100) {
                if (dx > 0)
                    x++;
                else
                    x--;
                frac &= ~0x100;
            }
        }
    } else {
        if (dx < 0) {
            t = x0;
            x0 = x1;
            x1 = t;
            t = y0;
            y0 = y1;
            y1 = t;
            dy = y1 - y0;
            dx = x1 - x0;
        }
        step = (y1 - y0 < 0 ? y0 - y1 : y1 - y0) * 256 / ((s32)(x1 - x0) < 0 ? (s32)(x0 - x1) : (s32)(x1 - x0));
        y = y0;
        x = x0;
        while (x != x1) {
            u32 tmp2;
            tmp2 = x >> 3;
            off = ((((y >> 3) << 4) + tmp2) << 3) + (y & 7);
            off = (off << 3) + (x & 7);
            if (buf[off] < color)
                buf[off] = color;
            frac += step;
            if (frac & 0x100) {
                if (dy > 0)
                    y++;
                else
                    y--;
                frac &= ~0x100;
            }
            x++;
        }
    }
}
