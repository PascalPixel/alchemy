#include "tundaria_tower.h"

void Func_0200137c(void) {
  if ((Actor_GetById(gSequenceParentWork.currentOwner)->y >> 19) != 0)
    MapCell_SetAttributes(0, 14, 17, 0);
  else
    MapCell_SetAttributes(0, 14, 17, 0x3200);
}
