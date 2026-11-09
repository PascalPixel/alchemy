#include "local.h"

extern const u8 MsgBlazeTabletInscription[];

void Func_020030cc(void);
void Func_02004d40(void);
void Func_02004d48(void);

void Func_020032d8(void) {
  if (Event_GetFlag(0x9fa)) {
    Func_02004d40();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 107);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 136);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}

void Func_02003348(void) {
  if (Event_GetFlag(0x9fa)) {
    Func_02004d40();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 280, 136);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}

void Func_020033b0(void) {
  if (Event_GetFlag(0x9fa)) {
    Func_02004d40();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 136);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}
