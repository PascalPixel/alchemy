#include "../../../../../../games/tla/src/scene/trial_road/trial_road.h"

void Func_02000768(void) {
  OBJECT_BYTE(Actor_GetById(17), 98) = 0;
  GameFlag_SetBit(SPLIT_CONSTANT(0x208, 255));
}

void Func_02000784(void) { Func_020059c0(); }

void Func_0200078c(s32 id) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL)
    Map_Stamp(14, 27, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_020007b4(void) {
  s32 y = Actor_GetById(gMainRoutineWork.currentOwner)->y;


  if (y == 0x200000)
    GameFlag_SetBit(0x208);
  else
    GameFlag_ClearBit(0x208);
}
