/* Draft, differ 50 of 40 listed (15 common), registers: the native copies the object to r5 on entry though it
 * calls nothing, keeps the map in r0 and adds the cells offset to it at once (adds r0, r0, r3), and reads z, y
 * and x through r5; this form keeps the object in r0 and the map in r5. The two stores, their reload of the
 * cells pointer and the clear of the mark agree in shape: probably an inline helper took the map and the
 * coordinates as arguments. */
#include "types.h"
#include "ram_buffer.h"

struct MapLayer {
    u8 unk00[0x30];
    u8 *cells;
    u8 unk34[4];
};

struct MapLayers {
    u8 unk000[0x108];
    struct MapLayer layers[3];
};

struct Object0200074c {
    u8 unk00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unk14[0x4f];
    u8 marked;
};

// Clears markers on the tiles ahead of and behind the object, stored in the
// third byte of each layer-two cell.
void Func_0200074c(struct Object0200074c *object)
{
    struct MapLayers *map;
    u8 **cells;
    s32 x;
    s32 z;

    if (object->marked == 0)
        return;
    map = (struct MapLayers *)Ram_HeapSlots->mapWork;
    if (map == NULL)
        return;
    cells = &map->layers[2].cells;
    z = (object->z - object->y) >> 20;
    x = object->x >> 20;
    (*cells)[((x + ((z + 1) << 7)) << 2) + 2] = 0;
    (*cells)[((x + ((z - 1) << 7)) << 2) + 2] = 0;
    object->marked = 0;
}
