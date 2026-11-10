#include "ankohl_ruins.h"

void Func_02000a68(s32 unused, s32 objectId) {
  s32 id = objectId;
  struct Actor *object = Actor_Get(id);
  s16 frames = 60;

  if (object->z >> 20 == 31) {
    object->motionFlags = 3;
    Func_02002e34(object, 3);
    Event_PlaySound(204);
    while (frames != 0) {
      System_Wait(1);
      if (object->velocityY == 0)
        break;
      frames--;
    }
    Actor_SetPosition(id, 0, 0);
    if (id == 10)
      Event_SetFlag(0x9d2);
    if (id == 11)
      Event_SetFlag(0x9d3);
    if (id == 12)
      Event_SetFlag(0x9d4);
    if (id == 13)
      Event_SetFlag(0x9d5);
    if (id == 14)
      Event_SetFlag(0x9d6);
    if (id == 15)
      Event_SetFlag(0x9d7);
    if (id == 16)
      Event_SetFlag(0x9d8);
  }
}
