#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Func_02003adc(s32 layer, s32 x, s32 z);
s32 Func_02003c24(s32 layer, s32 shape, s32 height);

s32 Func_020005cc(s32 layer, s32 x, s32 z, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = Func_02003adc(layer, x, z);
  u8 *out = FIXED_ADDRESS((u8 *), 0x02024000, gMapShapeGrid) +
            (map->layers[layer].cells - (struct MapCell *)Ram_MapCellBuffer);

  out += x + (z << 7);
  *out = Func_02003c24(layer, shape, height);
  return 0;
}
