/* DRAFT: 376 of 392 bytes, 183 differing halfwords. Draws a line into the
 * 8bpp tiled canvas (32 tiles wide) at *0x03001EF0 with an 8.8 fraction,
 * keeping the larger of the existing and new colour; the major axis runs from
 * the lower end. The same routine as Func_080cde90 without its clamp.
 * Remaining: allocation. Global-allocator priorities here are y0 .99,
 * y1 .82, x0 .51, dx .50, x1 .44 (floor_log2(refs) * refs / live), so dx
 * takes the caller-saved r4 and x1 r8; the reference gives dx r7, x1 r4 and
 * x0 r8, which needs x0 below both. With x1 in r4 the reference then saves
 * it around two divide calls on the steep path (8 bytes more). */
#include "TYPES.H"

s32 Math_Div(s32 value, s32 divisor);

#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_080f62b8(u32 x0, u32 y0, u32 x1, u32 y1, s32 color)
{
    u8 *buf = *(u8 **)0x03001ef0;
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;
    s32 frac = 0x80;
    s32 step;
    u32 x;
    u32 y;
    u32 t;
    u32 off;

    if (ABS(dx) < ABS(dy)) {
        if (dy < 0) {
            t = x0; x0 = x1; x1 = t;
            t = y0; y0 = y1; y1 = t;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        step = Math_Div(ABS((s32)(x1 - x0)) << 8, ABS((s32)(y1 - y0)));
        x = x0;
        for (y = y0; y != y1; y++) {
            off = ((((y >> 3) << 5) + (x >> 3)) << 3) + (y & 7);
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
        step = Math_Div(ABS((s32)(y1 - y0)) << 8, ABS((s32)(x1 - x0)));
        y = y0;
        for (x = x0; x != x1; x++) {
            off = ((((y >> 3) << 5) + (x >> 3)) << 3) + (y & 7);
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
