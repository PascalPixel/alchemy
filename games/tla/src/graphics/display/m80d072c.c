#include "types.h"
#include "irq.h"
#include "ram_buffer.h"

void Func_080d0954(void);

s32 Func_080d072c(void)
{
    s32 other = (s32)gIrqHandlers[1] ^ (s32)Func_080d0954;

    return 1 - ((u32)(-other | other) >> 31);
}

struct MapLayerFlags {
    u8 unknown_00[0x14];
    u16 flags;
};

/* FAKEMATCH: read as a volatile halfword the flags stay the AND's second operand, so the mask is built in the result register as in ⚓️; a plain temporary swaps the two registers (differ 24). */
#define MAP_LAYER_FLAGS(map) (*(volatile u16 *)&(map)->flags)

// マップの層ごとのフラグを落とす
void Map_ClearLayerFlags(s32 layer0, s32 layer1, s32 layer2)
{
    struct MapLayerFlags *map;
    u16 flags;

    map = (struct MapLayerFlags *)Ram_HeapSlots->map_work;
    if (map == 0)
        return;
    if (layer2) {
        flags = MAP_LAYER_FLAGS(map);
        asm volatile(""); /* FAKEMATCH: ⚓️ reads the flags before it starts the mask; written as map->flags &= ~0x200 the mask's first half is scheduled ahead of the read (differ 2) */
        map->flags = flags & ~0x200;
    }
    if (layer1) {
        flags = MAP_LAYER_FLAGS(map);
        asm volatile(""); /* FAKEMATCH: as above, for ~0x400 (differ 2) */
        map->flags = flags & ~0x400;
    }
    if (layer0) {
        flags = MAP_LAYER_FLAGS(map);
        asm volatile(""); /* FAKEMATCH: as above, for ~0x800 (differ 2) */
        map->flags = flags & ~0x800;
    }
}
