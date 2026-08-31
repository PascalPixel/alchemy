#include "types.h"
#include "map.h"

extern struct MapState *Data_03001e70;

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
    struct MapState *state = Data_03001e70;

    x >>= 20;
    y >>= 20;
    if (state != 0) {
        struct MapCell *cells = state->layers[layer & 3].cells;
        u32 offset = (x + (y << 7)) * sizeof(struct MapCell);
        u8 *cell = (u8 *)cells;

        cell += offset;
        cell[2] = collision_code;
    }
}
