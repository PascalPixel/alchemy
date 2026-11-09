#include "lemurian_ship.h"
#include "event_inline.h"

void Func_020010bc(void) { Func_02002dd0(gData_0200304e); }

void Func_020010cc(void) {
  Func_02002dd8();
  Func_02000e2c();
}

void Func_020010d8(void) {
  Sound_Play(106);
  Actor_SetModeById(8, 2);
  GameFlag_SetBit(0x204);
  Func_02000e2c();
}

void Func_020010f8(void) {
  Sound_Play(106);
  Actor_SetModeById(9, 2);
  GameFlag_SetBit(0x205);
  Func_02000e2c();
}

void Func_02001118(void) {
  Actor_SetPartAttribute(((struct LemurianShipObject *)Actor_GetById(10)), 0);
}

void Func_02001128(void) {
  Actor_SetPartAttribute(((struct LemurianShipObject *)Actor_GetById(11)), 0);
}

void Func_02001138(void) {
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(8));
  u32 i;

  Func_02002c68();
  Event_ResolvePendingActions(0);
  work->field16a = 0;
  GameFlag_ClearBit(0x202);
  GameFlag_ClearBit(0x204);
  Event_Wait(20);
  Sound_Play(186);
  for (i = 0; i <= 63; i++) {
    object->y += 0x4000;
    wait(1);
  }
  Actor_SetSpritePriority(8, 2);
  Map_CopyCellPictureIndicesRect(4, 123, 35, 69, 1, 1);
  MapGrid_CopyCellAttributes(36, 0, 1, 1, 35, 9);
  Sound_Play(188);
  MapGrid_CopyCellsTo(7, 124, 31, 64, 3, 2);
  GameFlag_ClearBit(0x982);
  Event_Wait(20);
  Func_02002c70();
}

void Func_020011f4(void) {
  struct LemurianShipEventWork *work = Ram_HeapSlots->eventWork;
  struct LemurianShipObject *object =
      ((struct LemurianShipObject *)Actor_GetById(9));
  u32 i;

  Func_02002c68();
  Event_ResolvePendingActions(0);
  work->field16a = 0;
  GameFlag_ClearBit(0x203);
  GameFlag_ClearBit(0x205);
  Event_Wait(20);
  Sound_Play(186);
  for (i = 0; i <= 63; i++) {
    object->y += 0x4000;
    wait(1);
  }
  Actor_SetSpritePriority(9, 2);
  Map_CopyCellPictureIndicesRect(4, 123, 29, 69, 1, 1);
  MapGrid_CopyCellAttributes(36, 0, 1, 1, 29, 9);
  Sound_Play(188);
  MapGrid_CopyCellsTo(7, 124, 31, 64, 3, 2);
  GameFlag_ClearBit(0x982);
  Event_Wait(20);
  Func_02002c70();
}
