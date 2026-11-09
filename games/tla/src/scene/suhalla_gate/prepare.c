#include "local.h"
#include "event_inline.h"

void Func_020002e0(void) {
  MapGrid_CopyCellAttributes(108, 38, 1, 1, 102, 56);
  GameFlag_SetBit(0x8de);
}

void Func_02000308(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  Actor_GetById(0)->motionFlags = 0;
  Sound_Play(158);
  Map_CopyCellPictureIndicesRect(66, 36, 71, 8, 2, 2);
  wait(4);
  Map_CopyCellPictureIndicesRect(68, 36, 71, 8, 2, 2);
  wait(4);
  Actor_CenterAndWalk(0, 3, -16);
  Sound_Play(123);
  Import_EventSetExitRequest(value);
}

void Func_02000378(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  gMainRoutineWork.currentOwner = 4;
  Sound_Play(123);
  Import_EventSetExitRequest(value);
}

s32 Scene_PrepareMap(void) { return 0; }
