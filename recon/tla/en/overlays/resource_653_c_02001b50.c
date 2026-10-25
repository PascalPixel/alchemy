// Draft, differ 38 of 133: sets flag 0x208 or 0x209 while the leader
// stands on a lift's cell (27, 12 or 37, 14), and 0x17d while on either.
// Left: register choice only (r5 and r6, r8 and r10 swapped).
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

void Func_02001b50(void) {
  struct ObjectRuntime *lift;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = leader->x;
  s32 z = leader->z;
  s32 cellX = x >> 20;
  s32 cellZ = z >> 20;
  s32 riding = 0;

  if (leader->y > 0x100000)
    return;
  if ((u32)(x - 0x1900000) > 0xf00000)
    return;
  if ((u32)(z - 0xb40000) > 0x640000)
    return;
  if (cellX == 27 && cellZ == 12) {
    lift = Object_GetById(13);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                       0);
    Object_SetModeById(13, 4);
    leader->motionFlags = riding;
    leader->y = lift->y;
    GameFlag_SetBit(0x208);
    riding = 1;
  }
  if (cellX == 37 && cellZ == 14) {
    lift = Object_GetById(14);
    ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                       0);
    Object_SetModeById(14, 4);
    leader->motionFlags = 0;
    leader->y = lift->y;
    GameFlag_SetBit(0x209);
    riding = 1;
  }
  if (riding) {
    GameFlag_SetBit(0x17d);
    return;
  }
  Object_SetModeById(13, 0);
  Object_SetModeById(14, 0);
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     1);
  leader->motionFlags = 3;
  GameFlag_ClearBit(0x208);
  GameFlag_ClearBit(0x209);
  GameFlag_ClearBit(0x17d);
}
