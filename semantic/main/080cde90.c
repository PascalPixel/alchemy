#include "types.h"

#define GRID_OFFSET(x, y) \
    (((((u32)(y) >> 3) * 16 + ((u32)(x) >> 3)) * 8 + \
      ((u32)(y) & 7)) * 8 + ((u32)(x) & 7))

s32 Func_080022ec(s32 value, s32 divisor);

/*
 * Rasterize a line into the 128x128 tiled intensity grid.  The grid is stored
 * as 16x16 tiles of 8x8 bytes, and a cell is only raised when the requested
 * intensity exceeds its current value.  The ending point is intentionally not
 * plotted, matching the reference DDA loops.
 */
void Func_080cde90(u32 x0, s32 y0, u32 x1, s32 y1, s32 intensity)
{
    u8 *grid = *(u8 **)0x03001ef0;
    s32 dx = (s32)(x1 - x0);
    s32 originalDy = y1 - y0;
    s32 absoluteDx = dx < 0 ? (s32)(0U - (u32)dx) : dx;
    s32 absoluteDy = originalDy < 0
        ? (s32)(0U - (u32)originalDy)
        : originalDy;
    u32 error = 0x80;

    if (y0 < 0)
        y0 = 0;
    if (y0 > 127)
        y0 = 127;
    if (y1 < 0)
        y1 = 0;
    if (y1 > 127)
        y1 = 127;

    if (absoluteDx < absoluteDy) {
        s32 step;

        if (originalDy < 0) {
            u32 swapX = x0;
            s32 swapY = y0;

            x0 = x1;
            x1 = swapX;
            y0 = y1;
            y1 = swapY;
            dx = (s32)(x1 - x0);
        }

        absoluteDx = dx < 0 ? (s32)(0U - (u32)dx) : dx;
        step = Func_080022ec((s32)((u32)absoluteDx << 8), y1 - y0);
        while (y0 != y1) {
            u32 offset = GRID_OFFSET(x0, y0);

            if ((s32)grid[offset] < intensity)
                grid[offset] = (u8)intensity;

            error += step;
            if ((error & 0x100) != 0) {
                if (dx > 0)
                    x0++;
                else
                    x0--;
                error &= ~0x100;
            }
            y0++;
        }
    } else {
        s32 dy;
        s32 step;

        if (dx < 0) {
            u32 swapX = x0;
            s32 swapY = y0;

            x0 = x1;
            x1 = swapX;
            y0 = y1;
            y1 = swapY;
            originalDy = y1 - y0;
            dx = (s32)(x1 - x0);
            dy = originalDy;
        } else {
            dy = y1 - y0;
        }

        absoluteDy = dy < 0 ? (s32)(0U - (u32)dy) : dy;
        if (dx >= 0) {
            step = Func_080022ec((s32)((u32)absoluteDy << 8), dx);
        } else {
            step = Func_080022ec(
                (s32)((u32)absoluteDy << 8), (s32)(x0 - x1));
        }
        while (x0 != x1) {
            u32 offset = GRID_OFFSET(x0, y0);

            if ((s32)grid[offset] < intensity)
                grid[offset] = (u8)intensity;

            error += step;
            if ((error & 0x100) != 0) {
                if (originalDy > 0)
                    y0++;
                else
                    y0--;
                error &= ~0x100;
            }
            x0++;
        }
    }
}
