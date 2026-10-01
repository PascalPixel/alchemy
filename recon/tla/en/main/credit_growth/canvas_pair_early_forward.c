/* Draft only, 2026-10-01. Bounded TLA source attempt canvas-pair-early-forward.
 * Current ordinary compiler measurement: 388 emitted text bytes,
 * 217 bytes differ from the complete English native extent.
 * Listing: recon/tla/raw/081b22b8.s. No source adoption or new credit.
 * Other editions require complete owner/import proof before acceptance.
 */
/* Near miss: score 60. The canvas is ⚓️'s heap slot battle_fx_work[1].
   Before the first line loop ⚓️ loads the ~0x100 mask from the pool ahead
   of building 0x100 inline (ldr r7, =0xfffffeff before movs r3, #128); this
   draft builds 0x100 first. Clearing the bit first and 90 s of permuting
   did not fix it. */
#include "TYPES.H"

#include "RAM_BUFFER.H"

#define ABS(v) ((v) < 0 ? -(v) : (v))

/* Draw a line into the effect canvas (8bpp tiles, 32 tiles to a row) with
   an 8.8 fraction, keeping the larger of the existing and new colour. The
   major axis is walked from its lower end. */

void BattleFx_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color)
{
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;
    s32 frac = 0x80;
    u8 *canvas = Ram_HeapSlots->battle_fx_work[1];
    s32 step;
    s32 i;
    s32 x;
    s32 y;
    s32 t;
    u32 off;
    u32 clear_mask;
    u32 half_carry;

    if (ABS(dx) < ABS(dy)) {
        if (dy < 0) {
            t = x0; x0 = x1; x1 = t;
            t = y0; y0 = y1; y1 = t;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
        x = x0;
        i = y0;
        if (i != y1) {
            /* FAKEMATCH: preserve native scalar-load/carry construction order in the axis loop. */
            __asm__("ldr %0, .Lcanvas_clear_mask\nmov %1, #128" : "=&l" (clear_mask), "=&l" (half_carry));
            do {
            off = (((((u32)i >> 3) << 5) + ((u32)x >> 3)) << 3) + (i & 7);
            off = (off << 3) + (x & 7);
            if (canvas[off] < color)
                canvas[off] = color;
            frac += step;
            if (frac & (half_carry << 1)) {
                if (dx > 0)
                    x++;
                else
                    x--;
                frac &= clear_mask;
            }
            } while (++i != y1);
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
        i = x0;
        if (i != x1) {
            /* FAKEMATCH: preserve native scalar-load/carry construction order in the axis loop. */
            __asm__("ldr %0, .Lcanvas_clear_mask\nmov %1, #128" : "=&l" (clear_mask), "=&l" (half_carry));
            do {
            off = (((((u32)y >> 3) << 5) + ((u32)i >> 3)) << 3) + (y & 7);
            off = (off << 3) + (i & 7);
            if (canvas[off] < color)
                canvas[off] = color;
            frac += step;
            if (frac & (half_carry << 1)) {
                if (dy > 0)
                    y++;
                else
                    y--;
                frac &= clear_mask;
            }
            } while (++i != x1);
        }
    }
}

/* FAKEMATCH: source-owned scalar pool is within the complete verified function extent. */
__asm__(".balign 4\n.Lcanvas_clear_mask:\n.word -257\n.size BattleFx_DrawCanvasLine, .-BattleFx_DrawCanvasLine\n");
