// Matches: differ 0 (tla-en, 2026-11-01)
// Writes a shape id into gMapShapeGrid at the actor's cell (16-pixel cells, 128
// to a row). A layer's cell pointer gives its offset in the buffer, in cells,
// which is the same offset in the shape grid. The two calls use the layer the
// actor stands on, not the parameter; Func_02003adc and Func_02003c24 stay
// unnamed. Plain C, no devices. Two forms decide it: z read into a local before
// the grid is added, so z's pseudo holds r2 and the grid takes r1 (with z read
// at the call the grid takes r2 and 6 instructions differ); and height >> 19
// and the offset as two statements, for `asrs; add r2, sl`. The same shape as
// Func_02000bc0 in resource_67f: only the two helpers' names should differ.
#include "types.h"
#include "actor.h"
#include "map.h"
#include "system/ram_buffer.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 Func_02003adc(s32 terrainId, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02003c24(s32 terrainId, s32 value, s32 height);

void Func_020036f8(s32 heightOffset, s32 layer, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 cell = Ram_HeapSlots->mapWork->layers[layer].cells -
             (struct MapCell *)Ram_MapCellBuffer;
  s32 z = actor->z;
  u8 *shapes = gMapShapeGrid + cell;
  s32 value;
  s32 height;

  value = Func_02003adc(actor->terrainId, actor->x, z);
  height = Map_GetTerrainHeight(actor->terrainId, actor->x, actor->z) >> 19;
  height += heightOffset;
  shapes += actor->x / 0x100000 + (actor->z / 0x100000 << 7);
  *shapes = Func_02003c24(actor->terrainId, value, height);
}
