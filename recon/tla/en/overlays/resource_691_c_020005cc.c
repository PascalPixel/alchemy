/* Near miss: differ 8 of 42. The draft adds the cell offset to the cell index before the grid address;
 * the listing adds the grid address first. */
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
s32 Func_02003adc(s32 layer, s32 x, s32 z);
s32 Func_02003c24(s32 layer, s32 shape, s32 height);

s32 Func_020005cc(s32 layer, s32 x, s32 z, s32 height)
{
    struct MapWork *map = Ram_HeapSlots->mapWork;
    s32 shape = Func_02003adc(layer, x, z);
    u8 *out = gMapShapeGrid + (map->layers[layer].cells - (struct MapCell *)0x02010000) + (x + (z << 7));

    *out = Func_02003c24(layer, shape, height);
    return 0;
}
