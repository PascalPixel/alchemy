#include "local.h"

void Func_02000428(s32 a) {
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Func_020028e0_(gMainRoutineWork.currentOwner, 6, 0x1000);
  Event_PlaySound(310);
  Event_SetFlag(0x122);
  FieldEvent_SetExitRequest(a);
}

void Func_02000464(s32 a, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 9) {
    if (object->x >> 20 == 17) {
      object->z += 0x10000;
      Func_02002764(2, 0x80000);
      Event_SetFlag(0xa6d);
    }
  } else if (id == 10) {
    if (object->x >> 20 == 29) {
      object->z += 0x10000;
      Func_02002764(8, 0x80000);
      Event_SetFlag(0xa6e);
    }
  }
}
