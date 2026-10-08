#include "types.h"
#include "ram_buffer.h"

struct MapWork {
    u8 unknown_00[0x14];
    u16 flags;
};

void Map_ClearLayerFlags(s32 layer0, s32 layer1, s32 layer2)
{
    struct MapWork *map;

    map = Ram_HeapSlots->map_work;
    if (map == 0)
        return;
    if (layer2)
        map->flags &= ~0x200;
    if (layer1)
        map->flags &= ~0x400;
    if (layer0)
        map->flags &= ~0x800;
}
