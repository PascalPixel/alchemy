#include "islets.h"

void Func_020024c8(void) {
  Call6((void (*)())Map_CopyCellAttributeRect, 5, 2, 1, 2, 5, 28);
  GameFlag_SetBit(0x213);
}

void Func_020024f0(void) {
  struct Actor *object;

  if (gInput.held & 0x40) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    Func_02003e64();
    Event_ResolvePendingActions(0);
    object->motionFlags = 0;
    Call3((void (*)())Actor_SetPositionAndReset, gMainRoutineWork.currentOwner,
          72, 280);
    object->motionFlags = 3;
    Func_02003e6c();
  }
}

void Func_0200253c(void) {
  struct Actor *object;
  u16 *state;

  if (gInput.held & 0x80) {
    state = gSceneState;
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if ((u16)(*state + 0xfff6) > 200) {
      Func_02003e64();
      Event_ResolvePendingActions(0);
      object->motionFlags = 0;
      Call3((void (*)())Actor_SetPositionAndReset,
            gMainRoutineWork.currentOwner, 72, 296);
      object->motionFlags = 3;
      Func_02003e6c();
    }
  }
}
