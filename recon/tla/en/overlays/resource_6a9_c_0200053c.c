// Matches: differ 0 (tla-en, 2026-11-01)
// One height variable serves every cell, so it lives across blocks: the shift
// stays in r0 and the sum goes to r3. The inlines keep each call's constants
// rematerialized and are needed, the flag ones included. Half reads the
// halfword as an int: a shortened s16 compare leaves 536 << 16, equal to
// 134 << 18, for GCSE to hoist above the branch.
#include "../../../../games/tla/src/scene/caverns/caverns.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020010cc(s32 x, s32 z, s32 layer, s32 height);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

static __inline__ s32 GetHeight(s32 x, s32 z) {
  return Map_GetTerrainHeight(2, x, z);
}

static __inline__ void SetHeight(s32 x, s32 z, s32 height) {
  Func_020010cc(x, z, 2, height);
}

static __inline__ s32 Half(u8 *base, s32 offset) {
  return HALF_AT(base, offset);
}

static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

static __inline__ void Flag_Set(s32 flag) { GameFlag_SetBit(flag); }

static __inline__ void Flag_Clear(s32 flag) { GameFlag_ClearBit(flag); }

#define SET_CELL_HEIGHT(x, z)                                                  \
  height = (GetHeight(x, z) >> 19) + offset;                                   \
  SetHeight(x, z, height)

void Func_0200053c(s32 offset) {
  u8 *work = *(u8 **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
  s32 height;

  offset <<= 3;
  if (HALF_AT(work, 14) > 31 || Flag_Test(0x213)) {
    SET_CELL_HEIGHT(142 << 18, 134 << 18);
    SET_CELL_HEIGHT(146 << 18, 138 << 18);
    if (offset == 8)
      Flag_Set(0x213);
    else
      Flag_Clear(0x213);
  } else if (Half(work, 18) == 536 || Flag_Test(0x213)) {
    SET_CELL_HEIGHT(134 << 18, 138 << 18);
    SET_CELL_HEIGHT(138 << 18, 138 << 18);
    SET_CELL_HEIGHT(142 << 18, 138 << 18);
    if (offset == 6)
      Flag_Set(0x213);
    else
      Flag_Clear(0x213);
  } else {
    SET_CELL_HEIGHT(134 << 18, 134 << 18);
    SET_CELL_HEIGHT(138 << 18, 134 << 18);
    SET_CELL_HEIGHT(142 << 18, 134 << 18);
  }
}
