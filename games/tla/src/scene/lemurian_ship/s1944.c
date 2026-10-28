#include "lemurian_ship.h"

void Func_02001944(s32 actor, s32 id) {
  struct LemurianShipObject *object = Actor_GetById(id);

  Func_02002c68();
  Func_02002da8(0);
  object->scaleX = 0x1000;
  object->scaleY = 0x1000;
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Func_02002de0(103);
  Actor_SetMovementTypeById(actor, gData_02002e30);
  Actor_SetMovementTypeAndWaitById(id, gData_02002e6c);
  FieldEvent_Wait(20);
  Func_02002c70();
}

void Func_020019ac(void) {
  if (GameFlag_Test(0x932) == 0) {
    Func_02002c68();
    Func_02002da8(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    FieldEvent_Wait(20);
    Func_02001a04();
    Func_02002c70();
  }
}

void Func_020019e0(void) {
  if (GameFlag_Test(0x932) == 0) {
    Func_02002c68();
    Func_02002da8(0);
    Func_02001a04();
    Func_02002c70();
  }
}

void Func_02001a04(void) {
  struct LemurianShipObject *object = Actor_GetById(9);

  Func_02002d40(9, 0x100, 0);
  Motion_SetVarCbAndRefresh(9, 2);
  object->field48 = 0x8000;
  Actor_SetSpritePriority(9, 1);
  LemurianShip_ObjSpeed(9, 0xcccc, 0x6666);
  Actor_Jump(9, 6, 0);
  LemurianShip_ObjCommit(9, 0x340, 88);
  FieldEvent_Wait(20);
  LemurianShip_ObjSpeed(9, 0x19999, 0xcccc);
  Actor_SetSpritePriority(9, 3);
  LemurianShip_ObjCommit(9, 0x338, 72);
  LemurianShip_ObjArm(9, 0x3000, 40);
  GameFlag_SetBit(0x201);
}
