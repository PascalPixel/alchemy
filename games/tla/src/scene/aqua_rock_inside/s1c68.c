#include "scene_tables.h"
#include "local.h"

void Func_02001c68(void) {
  if (Event_GetFlag(0x9ae)) {
    Func_02003d7c();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 152);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

void Func_02001cd8(void) {
  if (Event_GetFlag(0x9ae)) {
    Func_02003d7c();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 280, 184);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}

void Func_02001d40(void) {
  if (Event_GetFlag(0x9ae)) {
    Func_02003d7c();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgCheckedTheTabletIt, 1);
    Func_02003d84();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 184);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 184);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02001a1c();
  }
}
