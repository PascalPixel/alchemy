#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

void Func_02001b50(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x = leader->x;
  s32 cellX = x >> 20;
  s32 z = leader->z;
  s32 cellZ = z >> 20;
  s32 riding = 0;

  if (leader->y > 0x100000)
    return;
  if ((u32)(x - 0x1900000) > 0xf00000)
    return;
  if ((u32)(z - 0xb40000) > 0x640000)
    return;
  if (cellX == 27 && cellZ == 12) {
    struct Actor *lift = Actor_GetById(13);
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
    Actor_SetModeById(13, 4);
    leader->motionFlags = riding;
    leader->y = lift->y;
    GameFlag_SetBit(0x208);
    riding = 1;
  }
  if (cellX == 37 && cellZ == 14) {
    struct Actor *lift = Actor_GetById(14);
    Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                       0);
    Actor_SetModeById(14, 4);
    leader->motionFlags = 0;
    leader->y = lift->y;
    GameFlag_SetBit(SPLIT_CONSTANT(0x209));
    riding = 1;
  }
  if (riding) {
    GameFlag_SetBit(0x17d);
    return;
  }
  Actor_SetModeById(13, 0);
  Actor_SetModeById(14, 0);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     1);
  leader->motionFlags = 3;
  GameFlag_ClearBit(0x208);
  GameFlag_ClearBit(SPLIT_CONSTANT(0x209));
  GameFlag_ClearBit(0x17d);
}
