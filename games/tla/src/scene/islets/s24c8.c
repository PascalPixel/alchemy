#include "islets.h"
#include "event_inline.h"

void Func_020024c8(void) {
  MapGrid_CopyCellAttributes(5, 2, 1, 2, 5, 28);
  Event_SetFlag(0x213);
}

void Func_020024f0(void) {
  struct Actor *object;

  if (gInput.held & 0x40) {
    object = Actor_Get(gMainRoutineWork.currentOwner);
    Func_02003e64();
    FieldEvent_ResolvePendingActions(0);
    object->motionFlags = 0;
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 280);
    object->motionFlags = 3;
    Func_02003e6c();
  }
}

void Func_0200253c(void) {
  struct Actor *object;
  u16 *state;

  if (gInput.held & 0x80) {
    state = gSceneState;
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if ((u16)(*state + 0xfff6) > 200) {
      Func_02003e64();
      FieldEvent_ResolvePendingActions(0);
      object->motionFlags = 0;
      Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 72, 296);
      object->motionFlags = 3;
      Func_02003e6c();
    }
  }
}
