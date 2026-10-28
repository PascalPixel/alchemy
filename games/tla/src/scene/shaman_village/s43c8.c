#include "shaman_village.h"

void Func_020043c8(void) {
  s32 variant = Actor_GetById(gAdventure.currentOwner)->sprite->priority;

  Actor_SetSpritePriority(21, variant);
  Actor_SetSpritePriority(14, variant);
}
