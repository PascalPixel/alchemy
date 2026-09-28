/* Draft, not exact (2026-09-28): 392 of 392 bytes, 11 differing halfwords.
 * Draws a line into the 8bpp tiled canvas (32 tiles wide) at *0x03001EF0
 * with an 8.8 fraction, keeping the larger of the existing and new colour;
 * the major axis runs from the lower end. The same routine as Func_080cde90
 * without its clamp.
 * The step is (ABS(x1 - x0) << 8) / ABS(y1 - y0) on the macro's expression
 * arguments: fold distributes the division over the divisor's conditional,
 * which gives the reference its two __divsi3 calls with the r4 caller-save
 * on the steep path (cross-jumping merges them on the shallow path) and the
 * recomputed x0 - x1 numerator. Loading the canvas after dx, dy and the
 * fraction fixes the prologue schedule.
 * Residual, steep loop only: the reference counts y in r0 and steps x in r1,
 * while its shallow loop counts x in r0 and steps y in r1, so its loop
 * variables are not one x/y pair. Here x and y are single pseudos (r0/r1 in
 * both loops). Per-branch block variables, shared counter/stepper names and
 * declaration order each move the parameter and dx/dy allocation instead. */
#include "TYPES.H"

#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_080f62b8(s32 x0, s32 y0, s32 x1, s32 y1, s32 color)
{
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;
    s32 frac = 0x80;
    u8 *buf = *(u8 **)0x03001ef0;
    s32 step;
    s32 x;
    s32 y;
    s32 t;
    u32 off;

    if (ABS(dx) < ABS(dy)) {
        if (dy < 0) {
            t = x0; x0 = x1; x1 = t;
            t = y0; y0 = y1; y1 = t;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
        x = x0;
        for (y = y0; y != y1; y++) {
            off = (((((u32)y >> 3) << 5) + ((u32)x >> 3)) << 3) + (y & 7);
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
            t = x0; x0 = x1; x1 = t;
            t = y0; y0 = y1; y1 = t;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
        y = y0;
        for (x = x0; x != x1; x++) {
            off = (((((u32)y >> 3) << 5) + ((u32)x >> 3)) << 3) + (y & 7);
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
        }
    }
}
