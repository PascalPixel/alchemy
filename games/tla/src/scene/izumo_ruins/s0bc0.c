#include "types.h"
#include "actor.h"
#include "map.h"
#include "system/ram_buffer.h"
#include "event_inline.h"

extern u8 gMapShapeGrid[];

s32 Func_02001478(s32 terrainId, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02001528(s32 terrainId, s32 value, s32 height);

void Func_02000bc0(s32 heightOffset, s32 layer, s32 id) {
  struct Actor *actor = Actor_Get(id);
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
