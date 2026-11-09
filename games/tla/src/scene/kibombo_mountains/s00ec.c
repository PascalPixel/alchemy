#include "types.h"
#include "actor.h"
#include "event_inline.h"

void Func_020000ec(void) {
  struct Actor *object = Actor_GetById(11);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(24, 32, 1, 1, 22, 32);
  GameFlag_SetBit(0x221);
}

void Func_02000124(void) {
  Map_CopyCellAttributeRect(30, 35, 1, 1, 28, 34);
  Map_CopyCellAttributeRect(30, 34, 1, 1, 28, 33);
  GameFlag_SetBit(0x222);
}
