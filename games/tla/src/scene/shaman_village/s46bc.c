#include "shaman_village.h"

void Func_020046bc(void) {
  s32 variant = Actor_GetById(gMainRoutineWork.currentOwner)->sprite->priority;

  Actor_SetSpritePriority(9, variant);
  Actor_SetSpritePriority(10, variant);
}
