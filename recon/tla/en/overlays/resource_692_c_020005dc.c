// Under -mthumb-slow-loads (2026-10-26): differ 4 of 42; left: as overlay 691's
// Func_020005cc (resource_691_c_020005cc.c).
// Draft, differ 4 of 42: overlay 692's copy of the function drafted in
// resource_691_c_020005cc.c (overlay 691's Func_020005cc, differ 4 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "object_runtime.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

struct ObjectRuntime *Object_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
s32 Func_02004d20(s32 layer, s32 x, s32 z);
s32 Func_02004ef0(s32 layer, s32 shape, s32 height);

static inline u8 *ShapeGrid_ForCells(struct MapCell *cells) {
  return gMapShapeGrid + (cells - (struct MapCell *)0x02010000);
}

s32 Func_020005dc(s32 layer, s32 x, s32 z, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = Func_02004d20(layer, x, z);
  u8 *out = ShapeGrid_ForCells(map->layers[layer].cells) + (x + (z << 7));

  *out = Func_02004ef0(layer, shape, height);
  return 0;
}
