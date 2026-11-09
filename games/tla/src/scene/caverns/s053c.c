#include "../../../../games/tla/src/scene/caverns/caverns.h"
#include "event_inline.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020010cc(s32 x, s32 z, s32 layer, s32 height);

static __inline__ s32 GetHeight(s32 x, s32 z) {
  return Map_GetTerrainHeight(2, x, z);
}

static __inline__ void SetHeight(s32 x, s32 z, s32 height) {
  Func_020010cc(x, z, 2, height);
}

static __inline__ s32 Half(u8 *base, s32 offset) {
  return HALF_AT(base, offset);
}

#define SET_CELL_HEIGHT(x, z)                                                  \
  height = (GetHeight(x, z) >> 19) + offset;                                   \
  SetHeight(x, z, height)

void Func_0200053c(s32 offset) {
  u8 *work = *(u8 **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 height;

  offset <<= 3;
  if (HALF_AT(work, 14) > 31 || Event_GetFlag(0x213)) {
    SET_CELL_HEIGHT(142 << 18, 134 << 18);
    SET_CELL_HEIGHT(146 << 18, 138 << 18);
    if (offset == 8)
      Event_SetFlag(0x213);
    else
      Event_ClearFlag(0x213);
  } else if (Half(work, 18) == 536 || Event_GetFlag(0x213)) {
    SET_CELL_HEIGHT(134 << 18, 138 << 18);
    SET_CELL_HEIGHT(138 << 18, 138 << 18);
    SET_CELL_HEIGHT(142 << 18, 138 << 18);
    if (offset == 6)
      Event_SetFlag(0x213);
    else
      Event_ClearFlag(0x213);
  } else {
    SET_CELL_HEIGHT(134 << 18, 134 << 18);
    SET_CELL_HEIGHT(138 << 18, 134 << 18);
    SET_CELL_HEIGHT(142 << 18, 134 << 18);
  }
}
