// Under -mthumb-slow-loads (2026-10-26): differ 4 of 42; left: the shape grid's
// base address loads after the shift, where the native loads it into r2 before
// the first add.
// Near miss, differ 4 of 42: the shape grid entry of a layer cell. The grid
// offset of the cells pointer goes through an inline (permute found it): written
// in one expression the cell offset is added before the grid (8). Left: the
// native loads gMapShapeGrid (ldr r2) before it adds the cell buffer base,
// this draft after it into r3. Tried: the inline as an index or with the grid
// added last, a grid local behind a barrier (14), x + (z << 7) in other orders
// (27), permute 5 minutes.
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
s32 Func_02003adc(s32 layer, s32 x, s32 z);
s32 Func_02003c24(s32 layer, s32 shape, s32 height);

static inline u8 *ShapeGrid_ForCells(struct MapCell *cells) {
  return gMapShapeGrid + (cells - (struct MapCell *)0x02010000);
}

s32 Func_020005cc(s32 layer, s32 x, s32 z, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = Func_02003adc(layer, x, z);
  u8 *out = ShapeGrid_ForCells(map->layers[layer].cells) + (x + (z << 7));

  *out = Func_02003c24(layer, shape, height);
  return 0;
}
