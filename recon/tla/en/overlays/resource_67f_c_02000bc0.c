// Matches: differ 0 (tla-en, 2026-11-01)
// Writes a shape id into gMapShapeGrid at the actor's cell (16-pixel cells, 128
// to a row). The same function as overlay 691's Func_020036f8, whose draft
// explains the forms that decide it; only the two helpers' names differ. Plain
// C, no devices. Func_02001478 and Func_02001528 stay unnamed.
#include "types.h"
#include "actor.h"
#include "map.h"
#include "system/ram_buffer.h"

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 Func_02001478(s32 terrainId, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02001528(s32 terrainId, s32 value, s32 height);

void Func_02000bc0(s32 heightOffset, s32 layer, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 cell = Ram_HeapSlots->mapWork->layers[layer].cells -
             (struct MapCell *)Ram_MapCellBuffer;
  s32 z = actor->z;
  u8 *shapes = gMapShapeGrid + cell;
  s32 value;
  s32 height;

  value = Func_02001478(actor->terrainId, actor->x, z);
  height = Map_GetTerrainHeight(actor->terrainId, actor->x, actor->z) >> 19;
  height += heightOffset;
  shapes += actor->x / 0x100000 + (actor->z / 0x100000 << 7);
  *shapes = Func_02001528(actor->terrainId, value, height);
}
