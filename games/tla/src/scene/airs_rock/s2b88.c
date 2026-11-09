#include "types.h"
#include "actor.h"
#include "event_inline.h"

extern s32 gData_02004698;

void Func_02002b88(void) {
  struct Actor *actor = Actor_GetById(Party_GetLeaderActor());
  s32 level;

  actor->priorityFlags |= 32;
  level = ((actor->y + 0x200000) >> 16) / 112;
  if (gData_02004698 != level) {
    switch (level) {
  case 3:
    MapGrid_CopyCellAttributes(64, 64, 21, 3, 27, 54);
    break;
  case 2:
    MapGrid_CopyCellAttributes(64, 67, 21, 3, 27, 54);
    break;
  case 1:
    MapGrid_CopyCellAttributes(64, 70, 21, 3, 27, 54);
    break;
  case 0:
    MapGrid_CopyCellAttributes(64, 73, 21, 3, 27, 54);
    break;
    }
    gData_02004698 = level;
  }
}
