#include "jupiter_lighthouse_room_7.h"
#include "event_inline.h"

void Func_02000c7c(void) {
  struct Actor *object;
  s32 i;

  if (Event_GetFlag(0xa1a))
    Func_020009f8(20, 8, 12, 16, 0);
  Event_ClearFlag(0xa1a);
  Func_020005e8(1);
  System_Wait(2);
  object = Actor_Get(20);
  for (i = 0; i < 30; i++) {
    if (object->y <= object->terrainHeight && object->velocityY == 0)
      break;
    System_Wait(1);
  }
  object->motionFlags = 0;
}
