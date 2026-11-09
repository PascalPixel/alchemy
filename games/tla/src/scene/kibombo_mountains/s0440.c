#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_02000440(void) {
  struct Actor *object = Actor_Get(18);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(6, 16, 1, 1, 4, 16);
  Event_SetFlag(0x210);
}

void Func_02000478(void) {
  struct Actor *object = Actor_Get(19);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(8, 30, 1, 1, 10, 30);
  Event_SetFlag(0x211);
}
