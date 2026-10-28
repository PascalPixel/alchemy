#include "lemurian_ship.h"

void Func_020010bc(void) { Func_02002dd0(gData_0200304e); }

void Func_020010cc(void) {
  Func_02002dd8();
  Func_02000e2c();
}

void Func_020010d8(void) {
  Func_02002de0(106);
  Actor_SetModeById(8, 2);
  GameFlag_SetBit(0x204);
  Func_02000e2c();
}

void Func_020010f8(void) {
  Func_02002de0(106);
  Actor_SetModeById(9, 2);
  GameFlag_SetBit(0x205);
  Func_02000e2c();
}

void Func_02001118(void) { Actor_SetPartAttribute(Actor_GetById(10), 0); }

void Func_02001128(void) { Actor_SetPartAttribute(Actor_GetById(11), 0); }

void Func_02001138(void) {
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object = Actor_GetById(8);
  u32 i;

  Func_02002c68();
  Func_02002da8(0);
  work->field16a = 0;
  GameFlag_ClearBit(0x202);
  GameFlag_ClearBit(0x204);
  FieldEvent_WaitFrames(20);
  Func_02002de0(186);
  for (i = 0; i <= 63; i++) {
    object->y += 0x4000;
    WaitFrames(1);
  }
  Actor_SetSpritePriority(8, 2);
  LemurianShip_MapCopy(4, 123, 35, 69, 1, 1);
  LemurianShip_BlendStart(36, 0, 1, 1, 35, 9);
  Func_02002de0(188);
  LemurianShip_MapCopy(7, 124, 31, 64, 3, 2);
  GameFlag_ClearBit(0x982);
  FieldEvent_WaitFrames(20);
  Func_02002c70();
}

void Func_020011f4(void) {
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object = Actor_GetById(9);
  u32 i;

  Func_02002c68();
  Func_02002da8(0);
  work->field16a = 0;
  GameFlag_ClearBit(0x203);
  GameFlag_ClearBit(0x205);
  FieldEvent_WaitFrames(20);
  Func_02002de0(186);
  for (i = 0; i <= 63; i++) {
    object->y += 0x4000;
    WaitFrames(1);
  }
  Actor_SetSpritePriority(9, 2);
  LemurianShip_MapCopy(4, 123, 29, 69, 1, 1);
  LemurianShip_BlendStart(36, 0, 1, 1, 29, 9);
  Func_02002de0(188);
  LemurianShip_MapCopy(7, 124, 31, 64, 3, 2);
  GameFlag_ClearBit(0x982);
  FieldEvent_WaitFrames(20);
  Func_02002c70();
}
