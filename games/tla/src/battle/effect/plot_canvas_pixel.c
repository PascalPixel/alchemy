#include "types.h"
#include "ram_buffer.h"

void BattleFx_PlotCanvasPixel(s32 x, s32 y, s32 color)
{
    u16 *map = Ram_HeapSlots->canvas_map;
    u8 *canvas = Ram_HeapSlots->battle_fx_work[1];

    if (x > 63) {
        canvas += 0x1000;
        x -= 64;
    }
    canvas[map[(y << 6) + x]] = color;
}
