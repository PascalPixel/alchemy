/* Draft, differ 66 of 67: the native keeps each u16 counter sign-extended in
 * a register and tests its zero-extended copy, indexes rows by (u16)z * 512,
 * and tests 0 against the bounds at entry; agscc keeps z << 16 instead. Tried:
 * s16 counters with u16 casts (68), pointer rows, two minutes of alchemy
 * permute (42 at best). */
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/ram_buffer.h"
#include "../../../../games/tla/include/map.h"

void Func_0200495c(s32 layer, s32 keep)
{
    struct MapWork *map = Ram_HeapSlots->mapWork;
    struct MapLayer *mapLayer = &map->layers[layer];
    u32 *cells = (u32 *)mapLayer->cells;
    s32 width = (map->maxX - map->minX) >> 20;
    s32 height = (map->maxZ - map->minZ) >> 20;
    u32 fill;
    u16 z;
    u16 x;

    cells += ((map->minZ >> 20) << 7) + (map->minX >> 20);
    fill = *cells;
    for (z = 0; z < height; z++) {
        u32 *cell = &cells[z * 128];

        for (x = 0; x < width; x++) {
            if (((u8 *)cell)[2] != 0 && ((u8 *)cell)[2] != keep)
                *cell = fill;
            cell++;
        }
    }
}
