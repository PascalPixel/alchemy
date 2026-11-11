#include "types.h"
#include "ram_buffer.h"

struct GaiaRockInsideLayer {
  u8 unk00[8];
  s32 x;
  s32 z;
  u8 unk10[0x28];
};

struct GaiaRockInsideMapWork {
  u8 unk000[0x108];
  struct GaiaRockInsideLayer layers[2];
};

extern u8 gData_020023c4[];
extern s8 gData_020053fc[][2];

void Func_02004e0c(s32 x, s32 z, s32 width, s32 height, s32 cellX, s32 cellZ);

/// From the third state on, for each of the two map layers: Func_02004e0c
/// with the state's origin plus the layer's cell, a 15 by 10 size and the
/// layer's cell plus one.
void Func_020048a0(void) {
  s32 state = *(s32 *)(gData_020023c4 + 0x288);
  s8 *origin = gData_020053fc[state];
  struct GaiaRockInsideMapWork *map =
      (struct GaiaRockInsideMapWork *)Ram_HeapSlots->mapWork;

  if (state > 1) {
    s32 originX = origin[0];
    s32 originZ = origin[1];
    struct GaiaRockInsideLayer *layer = map->layers;
    s32 i;

    for (i = 1; i >= 0; i--, layer++) {
      /* FAKEMATCH(pin): x fixed to r2, so x is held in r2 and z in r3; as a
       * plain local, 18 of 45 instructions differ in tla-en. */
      register s32 x asm("r2") = layer->x >> 20;
      s32 z = layer->z >> 20;

      Func_02004e0c(originX + x, originZ + z, 15, 10, x + 1, z + 1);
    }
  }
}
