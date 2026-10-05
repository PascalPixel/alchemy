/* Near miss, differ 2: the native loads the canvas map before shifting y;
   this draft shifts first. Operand order, declaration order and an index
   local did not move it. Joins PLOT_CANVAS_PIXEL.C on a match. */
#include "TYPES.H"
#include "RAM_BUFFER.H"

/* Sets one pixel of the left half of the battle canvas. */
void BattleFx_PlotHalfCanvasPixel(s32 x, s32 y, s32 color)
{
    u16 *map = Ram_HeapSlots->canvas_map;
    u8 *canvas = Ram_HeapSlots->battle_fx_work[1];

    canvas[map[(y << 6) + x]] = color;
}
