/* Near miss, differ 28 of 66: the tail differs in register choice. The native
 * keeps the scaled code in r1, reads the kind through r0, parks the handler
 * offset in ip and loads the handler last; this reads the kind through r3. */
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapShapeGrid[];
extern s32 (*const Map_TerrainHeightHandlers[])(u8 *shape, s32 x, s32 z);

s32 Func_0802d71c(s32 px, s32 pz, s32 layer, s32 base)
{
    s32 x = px >> 16;
    s32 z = pz >> 16;
    struct MapWork *work = Ram_HeapSlots->map_work;
    u8 *state = Ram_HeapSlots->event_work;
    u8 *grid;
    s32 code;

    if (work == NULL)
        return 0;
    if (*(s8 *)(state + 0x18a) == 3)
        return 0;
    if (layer <= 2)
        grid = work->layers[layer].terrain_ids;
    else
        grid = gMapShapeGrid;
    code = grid[x / 16 + ((z / 16) << 7)] * 4;
    return Map_TerrainHeightHandlers[Ram_MapCollision[code] & 15](Ram_MapCollision + 1 + code, x & 15, z & 15) - base;
}
