#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "MAP.H"

extern u8 gMapCellBuffer[];

u8 GetMapCellCollision(s32 layer, s32 x, s32 y)
{
    struct MapWork *state;
    struct MapCell *cells;

    state = Ram_HeapSlots->map_work;
    x >>= 20;
    y >>= 20;
    cells = (struct MapCell *)gMapCellBuffer;
    if (state != NULL) {
        cells = state->layers[layer & 3].cells;
    }
    cells += x + (y << 7);
    return cells->collision_code;
}

void SetMapCellCollision(u32 layer, s32 x, s32 y, u32 collision_code)
{
    struct MapWork *state = Ram_HeapSlots->map_work;

    x >>= 20;
    y >>= 20;
    if (state != NULL) {
        struct MapCell *cells = state->layers[layer & 3].cells;

        cells += x + (y << 7);
        cells->collision_code = collision_code;
    }
}
