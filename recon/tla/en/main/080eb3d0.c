/* Draft, differ 57 of 96 (2026-10-15): the midpoint circle, eight
 * BattleFx_PlotHalfCanvasPixel calls per step; belongs after Func_080eb318
 * in plot_canvas_pixel.c. Every instruction is the native's except the
 * registers: global allocation ranks x (18 weighted refs) above color (17,
 * same live length), so x takes r7 and color r10, where the native has
 * color in r7 and x in r10 (all the rest follows). Measured: the decision
 * term as its own local (differ 79), y declared first, a for loop, the
 * x decrement as --x * 2, (x - 1) * 2, x + x and x * 2 - 2, the plots
 * through an inline of four, and pins of x to r10 (102) or color to r7
 * (80); five minutes of permuter found nothing sound. */
#include "types.h"
#include "ram_buffer.h"

void BattleFx_PlotHalfCanvasPixel(s32 x, s32 y, s32 color);

void Func_080eb3d0(s32 cx, s32 cy, s32 radius, s32 color)
{
    s32 x = radius;
    s32 y = 0;

    while (x >= y) {
        BattleFx_PlotHalfCanvasPixel(cx + x, cy + y, color);
        BattleFx_PlotHalfCanvasPixel(cx + x, cy - y, color);
        BattleFx_PlotHalfCanvasPixel(cx - x, cy + y, color);
        BattleFx_PlotHalfCanvasPixel(cx - x, cy - y, color);
        BattleFx_PlotHalfCanvasPixel(cx + y, cy + x, color);
        BattleFx_PlotHalfCanvasPixel(cx + y, cy - x, color);
        BattleFx_PlotHalfCanvasPixel(cx - y, cy + x, color);
        BattleFx_PlotHalfCanvasPixel(cx - y, cy - x, color);
        radius = radius - y * 2 + 1;
        if (radius < 0) {
            x--;
            radius += x * 2;
        }
        y++;
    }
}
