#include "lemurian_ship.h"
#include "event_inline.h"
#include "scene_tables.h"

void Func_02001944(s32 actor, s32 id) {
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(id));

  Func_02002c68();
  Event_ResolvePendingActions(0);
  object->scaleX = 0x1000;
  object->scaleY = 0x1000;
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Sound_Play(103);
  Actor_SetMovementTypeById(actor, gData_02002e30);
  Actor_SetMovementTypeAndWaitById(id, gData_02002e6c);
  Event_Wait(20);
  Func_02002c70();
}

void Func_020019ac(void) {
  if (GameFlag_Test(0x932) == 0) {
    Func_02002c68();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(20);
    Func_02001a04();
    Func_02002c70();
  }
}

void Func_020019e0(void) {
  if (GameFlag_Test(0x932) == 0) {
    Func_02002c68();
    Event_ResolvePendingActions(0);
    Func_02001a04();
    Func_02002c70();
  }
}

void Func_02001a04(void) {
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(9));

  Func_080d47b4(9, 0x100, 0);
  Motion_SetVarCbAndRefresh(9, 2);
  object->field48 = 0x8000;
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpeed(9, 0xcccc, 0x6666);
  Actor_Jump(9, 6, 0);
  Actor_SetPositionAndCommit(9, 832, 88);
  Event_Wait(20);
  Actor_SetSpeed(9, 0x19999, 0xcccc);
  Actor_SetSpritePriority(9, 3);
  Actor_MoveToAndWait(9, 824, 72);
  Actor_FaceDirection(9, FACING_SOUTHEAST + FACING_STEP, 40);
  GameFlag_SetBit(0x201);
}
