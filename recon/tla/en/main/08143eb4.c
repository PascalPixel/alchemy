/* Near miss, differ 4 of 197: The Broken Seal's BattleFx_DrawClippedCanvasLine with
   ⚓️'s fraction in bit 31 (start 0x80 << 23, step << 23, carry when negative, then
   cleared with a shift pair) and dx/dy reused as the step direction. Left: at entry
   the native loads the canvas pointer before it copies y0 to r5 (ldr r3, [r3, #96];
   adds r5, r1, #0), and in the second loop it reloads the spilled canvas two rows
   earlier (ldr r6, [sp, #0] before adds r3, r0, #0). Tried: a one-pass block around
   the canvas load (differ 12), an operand barrier on it (differ 6), alchemy permute
   for 6 minutes (no better). */
#include "types.h"
#include "ram_buffer.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

void Func_08143eb4(s32 x0, s32 y0, s32 x1, s32 y1, s32 color)
{
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;
    s32 frac = 0x80 << 23;
    u8 *canvas = Ram_HeapSlots->battle_fx_work[1];
    s32 step;
    s32 i;
    s32 x;
    s32 y;
    s32 tmp;
    u32 offset;

    if (y0 < 0)
        y0 = 0;
    if (y0 > 127)
        y0 = 127;
    if (y1 < 0)
        y1 = 0;
    if (y1 > 127)
        y1 = 127;

    if (ABS(dx) < ABS(dy))
    {
        if (dy < 0)
        {
            tmp = x0;
            x0 = x1;
            x1 = tmp;
            tmp = y0;
            y0 = y1;
            y1 = tmp;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        step = (ABS(x1 - x0) << 8) / ABS(y1 - y0);
        step <<= 23;
        x = x0;
        if (dx < 0)
            dx = -1;
        else
            dx = 1;
        for (i = y0; i != y1; i++)
        {
            offset = (((((u32)i >> 3) << 4) + ((u32)x >> 3)) << 3) + (i & 7);
            offset = (offset << 3) + (x & 7);
            if (canvas[offset] < color)
                canvas[offset] = color;
            frac += step;
            if (frac < 0)
            {
                x += dx;
                frac = (u32)(frac << 1) >> 1;
            }
        }
    }
    else
    {
        if (dx < 0)
        {
            tmp = x0;
            x0 = x1;
            x1 = tmp;
            tmp = y0;
            y0 = y1;
            y1 = tmp;
            dx = x1 - x0;
            dy = y1 - y0;
        }
        step = (ABS(y1 - y0) << 8) / ABS(x1 - x0);
        step <<= 23;
        y = y0;
        if (dy < 0)
            dy = -1;
        else
            dy = 1;
        for (i = x0; i != x1; i++)
        {
            offset = (((((u32)y >> 3) << 4) + ((u32)i >> 3)) << 3) + (y & 7);
            offset = (offset << 3) + (i & 7);
            if (canvas[offset] < color)
                canvas[offset] = color;
            frac += step;
            if (frac < 0)
            {
                y += dy;
                frac = (u32)(frac << 1) >> 1;
            }
        }
    }
}
