#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern struct MapCell gMapBlocks[];
extern struct MapCell gMapCellBuffer[];

s32 GetWorldMapCollisionFar(s32 *position);

struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer)
{
    u8 *event = Ram_HeapSlots->event_work;
    struct MapWork *map;
    struct MapCell *cells;

    /* FAKEMATCH: ⚓️ loads the event work before it builds its offset */
    asm volatile("");
    map = Ram_HeapSlots->map_work;

    if (*(s8 *)(event + 394) == 3) {
        cells = &gMapBlocks[((x / 0x200000) & 31) + ((z / 0x200000) & 31) * 32];
    } else {
        if (layer <= 2)
            cells = map->layers[layer].cells;
        else
            cells = gMapCellBuffer;
        cells = &cells[x / 0x100000 + (z / 0x100000) * 128];
    }
    return cells;
}
