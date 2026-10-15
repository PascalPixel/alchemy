#include "islets.h"

void Func_02003790(s32 layer, s32 x, s32 z, u32 width, u32 height, s32 value)
{
    struct MapCell *cells = Ram_HeapSlots->map_work->layers[layer].cells;
    struct MapCell *cell;
    u32 i;
    u32 j;

    cells += x + z * 128;
    for (i = 0; i < height; i++) {
        cell = cells + i * 128;
        for (j = 0; j < width; j++) {
            cell->collision_code = value;
            cell++;
        }
    }
}
