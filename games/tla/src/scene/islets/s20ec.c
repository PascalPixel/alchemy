#include "islets.h"

void Func_020020ec(s32 x, s32 z, s32 layers, s32 value)
{
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct MapCell *cells0 = map->layers[0].cells;
    struct MapCell *cells2 = map->layers[2].cells;
    s32 index = x / 0x100000 + (z / 0x100000) * 128;

    if (layers & 1) {
        cells0 += index;
        cells0->collisionCode = value;
    }
    if (layers & 2) {
        cells2 += index;
        cells2->collisionCode = value;
    }
}
