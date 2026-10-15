/* Near miss: differ 2 of 188. Ahead of the first loop the native loads the mask 0xfffffeff from the pool before it builds 0x100. loop.c hoists 7 and 0x100 in its first pass and the mask only in its second ("not desirable" at life 1), so the mask comes last in the preheader and sched2 breaks the priority-3 tie by insn order. Hand-hoisting the mask after the entry test orders the pair but gives the mask lr and 0x100 r8 (differ 12); hand-hoisting both, 0x100 first, gives the right registers in the wrong order (differ 4); pinning both (mask r8, 0x100 lr) and setting the mask first swaps their reload registers r3 and r7 (differ 10). Five permuter minutes found nothing better. */
#include "types.h"

#include "ram_buffer.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

void BattleFx_DrawCanvasLine(s32 x0, s32 y0, s32 x1, s32 y1, s32 color)
{
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;
    s32 frac = 0x80;
    u8 *canvas = Ram_HeapSlots->battleFxWork[1];
    s32 step;
    s32 i;
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
        for (i = y0; i != y1; i++) {
            off = (((((u32)i >> 3) << 5) + ((u32)x >> 3)) << 3) + (i & 7);
            off = (off << 3) + (x & 7);
            if (canvas[off] < color)
                canvas[off] = color;
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
        for (i = x0; i != x1; i++) {
            off = (((((u32)y >> 3) << 5) + ((u32)i >> 3)) << 3) + (y & 7);
            off = (off << 3) + (i & 7);
            if (canvas[off] < color)
                canvas[off] = color;
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
