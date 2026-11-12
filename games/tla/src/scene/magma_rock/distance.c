#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"
#include "map.h"

struct EventObjects {
  u8 unk00[20];
  struct Actor *objects[64];
};

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);

void Func_02000594(void) { Func_080d2b4c(8, 56); }

s32 Func_020005a0(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

extern u8 gMapCellBuffer[];
extern u8 gMapShapeGrid[];

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02003adc(s32 layer, s32 x, s32 z);
s32 Func_02003c24(s32 layer, s32 shape, s32 height);

s32 Func_020005cc(s32 layer, s32 x, s32 z, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 shape = Func_02003adc(layer, x, z);
  u8 *out = Ram_MapShapeGrid +
            (map->layers[layer].cells - (struct MapCell *)Ram_MapCellBuffer);

  out += x + (z << 7);
  *out = Func_02003c24(layer, shape, height);
  return 0;
}
