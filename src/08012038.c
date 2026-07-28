#include "types.h"
#include "map.h"

u8 Func_08012038(s32 layer, s32 x, s32 y)
{
    struct MapState *state;
    s32 cell_address;
    s32 layer_offset;

    state = *(struct MapState **)0x03001E70;
    x >>= 20;
    y >>= 20;
    cell_address = 0x02010000;
    if (state != 0) {
        layer_offset = ((layer & 3) * sizeof(struct MapLayer)) + 0x130;
        cell_address = *(s32 *)((u8 *)state + layer_offset);
    }
    cell_address += (x + (y << 7)) * sizeof(struct MapCell);
    return ((struct MapCell *)cell_address)->collision_code;
}
