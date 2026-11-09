#include "types.h"
#include "ram_buffer.h"
#include "map.h"

s32 GetWorldMapCollisionFar(s32 *position);

struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);

s32 Func_080dbd5c(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;

  if (*(s8 *)(event + 394) == 3) {
    s32 position[3];
    s32 collision;

    position[1] = 0;
    position[0] = x;
    position[2] = z;
    collision = GetWorldMapCollisionFar(position);
    return (u32)(collision - 2) <= 2;
  }
  return (u8)(Func_080dbb78(x, z, layer)->attributeB & 16);
}
