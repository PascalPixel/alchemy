/*
 * Draft: Map_ClearLayerFlags does not yet match; the reference loads the
 * flags before building each mask, here the mask comes first (3 units per
 * test). Plain temporaries and an asm pin reshape the whole function.
 * Links as recon/tla/raw/080d0184.s.
 */
#include "TYPES.H"
#include "RAM_BUFFER.H"

struct MapWork {
    u8 unknown_00[0x14];
    u16 flags;
};

/* Clears the map's layer flags 0x800, 0x400 and 0x200 for each of its
   arguments that is set, the last first; without a map it does nothing. */
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
