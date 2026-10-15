/* Draft, differ 11 of 60: the native subtracts the cell buffer base (pool 0xfdff0000) before it loads gMapShapeGrid and adds the grid after the shift; this draft loads the grid first, and copies the height before its shift (adds r2, r0, #0; asrs r2, r2, #19 for asrs r2, r0, #19). Tried: the index as a local (36), a cells local, array indexing, the grid added later (15), permute 1 minute. */
#include "../../../../games/tla/src/field/overlay_68f/ov68f.h"
#include "map.h"

extern u8 gMapShapeGrid[];

s32 Func_02003010(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02003180(s32 layer, s32 shape, s32 height);

void Func_02002c1c(s32 level, s32 layer, s32 owner)
{
    struct ObjectRuntime *object = Object_GetById(owner);
    u8 *shape = gMapShapeGrid + (Ram_HeapSlots->map_work->layers[layer].cells - (struct MapCell *)Ram_MapCellBuffer);
    s32 value = Func_02003010(object->terrain_id, object->x, object->z);
    s32 height = Map_GetTerrainHeight(object->terrain_id, object->x, object->z);

    height >>= 19;
    height += level;

    shape += object->x / 0x100000 + object->z / 0x100000 * 128;
    *shape = Func_02003180(object->terrain_id, value, height);
}
