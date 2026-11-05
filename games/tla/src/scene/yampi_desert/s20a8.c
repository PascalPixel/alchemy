#include "scene.h"

void Func_020020a8(void) {
  struct FieldActor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (leader->y > -0x100000)
    gMainRoutineWork.unk2c3 = 1;
  else
    gMainRoutineWork.unk2c3 = 0;
}
