#include "shaman_village.h"

void Func_02003b24(void) {
  Actor_SetSpritePriority(6, 1);
  Sound_Play(131);
  Func_02004a54();
  Func_02004a6c(((struct ShamanVillageObject *)Actor_GetById(6)), 2);
  Event_Wait(40);
  Func_02004a6c(((struct ShamanVillageObject *)Actor_GetById(6)), 0);
  Func_02004a64();
  Func_02004a5c();
  Func_0200057c();
  Actor_SetSpritePriority(6, 2);
}

void Func_02003b6c(s32 a, s32 b, s32 c, s32 d, s32 mode, s32 f) {
  struct ShamanVillageEventWork *work = Ram_HeapSlots->eventWork;
  struct ShamanVillageObject *object;
  s32 cell;

  Func_020048c4();
  Event_ResolvePendingActions(0);
  if (mode == 2)
    Sound_Play(188);
  else
    Sound_Play(158);
  Map_CopyCellPictureIndicesRect(a, b, c, d, mode, f);
  Event_Wait(20);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = ((struct ShamanVillageObject *)Actor_GetById(gMainRoutineWork.currentOwner));
    cell = object->x >> 16;
    Actor_SetMoveTarget(object, ((8 - cell % 16) << 16) + object->x, object->y,
                  object->z);
    Script_WaitForEventTimeout(object);
    object->facing = 0xc000;
  }
  Actor_SetMovementTypeById(gMainRoutineWork.currentOwner,
                                          gData_02004c08);
  wait(12);
  Sound_Play(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->exit);
}

void Func_02003c58(void) { Func_02003b6c(43, 40, 8, 22, 1, 2); }

void Func_02003c74(void) { Func_02003b6c(43, 42, 4, 26, 1, 2); }

void Func_02003c90(void) { Func_02003b6c(43, 42, 27, 21, 1, 2); }

void Func_02003cac(void) { Func_02003b6c(43, 40, 28, 17, 1, 2); }

void Func_02003cc8(void) { Func_02003b6c(43, 42, 10, 8, 1, 2); }
