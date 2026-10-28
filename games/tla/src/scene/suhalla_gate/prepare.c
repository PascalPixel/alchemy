#include "local.h"

void Func_020002e0(void) {
  Func_02000978_(108, 38, 1, 1, 102, 56);
  GameFlag_SetBit(0x8de);
}

void Func_02000308(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  Object_GetById(0)->motionFlags = 0;
  Func_02000aa0(158);
  Func_02000970(66, 36, 71, 8, 2, 2);
  WaitFrames(4);
  Func_02000970(68, 36, 71, 8, 2, 2);
  WaitFrames(4);
  ObjectMotion_CenterOnColumn_(0, 3, -16);
  Func_02000aa0(123);
  Func_02000a38(value);
}

void Func_02000378(void) {
  s32 value = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  gAdventure.currentOwner = 4;
  Func_02000aa0(123);
  Func_02000a38(value);
}

s32 Scene_PrepareMap(void) { return 0; }
