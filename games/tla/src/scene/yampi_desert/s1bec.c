#include "scene.h"

void Func_02001bec(void) {
  struct FieldActor *object;

  Func_02004040();
  Func_020041b8(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_SetModeById(gSequenceParentWork.currentOwner, 28);
  object = Func_02001b70();
  PartyInventory_GiveItem(209, 0);
  if (object != 0) {
    Actor_SetPositionAndResetMotion((struct Actor *)object, 0, 0, 0);
    wait(1);
    Func_02003fc8(object);
  }
  Actor_SetModeById(gSequenceParentWork.currentOwner, 1);
  GameFlag_SetBit(0x88c);
  Func_02004048();
}
