/* DRAFT: 404 of 420 bytes, 178 differing halfwords. Draws a line into the
 * 8bpp tiled canvas (16 tiles wide) at *0x03001EF0 with an 8.8 fraction,
 * clamping y to 0..127 after the deltas are taken.
 * Remaining: allocation, as in Func_080f62b8. Priorities from the lreg dump:
 * y0 .99, y1 .82, x0 .51 (12 refs over 70 insns), dx .50, x1 .44; the
 * reference needs dx and x1 above x0 (dx r7, x1 caller-saved r4, x0 r8),
 * and then duplicates the shallow path's divide around the r4 save. Goto
 * loops and spelling the ABS arms through dx did not move the counts. */
#include "TYPES.H"

s32 Math_Div(s32 value, s32 divisor);

#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_080cde90(u32 x0, s32 y0, u32 x1, s32 y1, s32 color)
{
    u8 *buf = *(u8 **)0x03001ef0;
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;
    s32 frac = 0x80;
    s32 step;
    u32 x;
    u32 y;
    s32 t;
    u32 off;

    if (y0 < 0)
        y0 = 0;
    if (y0 > 127)
        y0 = 127;
    if (y1 < 0)
        y1 = 0;
    if (y1 > 127)
        y1 = 127;
    if (ABS(dx) < ABS(dy)) {
        if (dy < 0) {
            t = x0; x0 = x1; x1 = t;
            t = y0; y0 = y1; y1 = t;
            dx = x1 - x0;
        }
        step = Math_Div(ABS((s32)(x1 - x0)) << 8, ABS(y1 - y0));
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
            t = x0; x0 = x1; x1 = t;
            t = y0; y0 = y1; y1 = t;
            dy = y1 - y0;
            dx = x1 - x0;
        }
        step = Math_Div(ABS(y1 - y0) << 8, ABS((s32)(x1 - x0)));
        y = y0;
        for (x = x0; x != x1; x++) {
            off = ((((y >> 3) << 4) + (x >> 3)) << 3) + (y & 7);
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
