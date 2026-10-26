#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "macros.h"

s32 GetWorldMapCollisionFar(s32 *position);
struct MapCell *Func_080dbb78(s32 x, s32 z, s32 layer);

s32 Func_080dbcd8(s32 x, s32 z, s32 layer) {
  u8 *event = Ram_HeapSlots->eventWork;
  s32 column = x;
  if (*(s8 *)(event + 394) == 3) {
    s32 position[3];

    SET_VECTOR(position, column, 0, z);
    column = (u32)(GetWorldMapCollisionFar(position) - 5) <= 1;
  } else {
    s32 code = Func_080dbb78(column, z, 2)->collisionCode;

    code ^= 232;
    column = (u32)(-code | code) >> 31;
    column = 1 - column;
  }
  if (*(s8 *)(event + 0xd37) != 0)
    column = 1 - ((u32)(-column | column) >> 31);
  return column;
}
