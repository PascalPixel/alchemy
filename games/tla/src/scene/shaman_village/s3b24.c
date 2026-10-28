#include "shaman_village.h"

void Func_02003b24(void) {
  Actor_SetSpritePriority(6, 1);
  Import_SoundPlay(131);
  Func_02004a54();
  Func_02004a6c(Actor_GetById(6), 2);
  FieldEvent_WaitFrames(40);
  Func_02004a6c(Actor_GetById(6), 0);
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
  Func_02004a24(0);
  if (mode == 2)
    ShamanVillage_SePlay(188);
  else
    ShamanVillage_SePlay(158);
  ShamanVillage_MapCopy(a, b, c, d, mode, f);
  FieldEvent_WaitFrames(20);
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0x8000, 0x4000);
  if (mode == 1) {
    object = Actor_GetById(gAdventure.currentOwner);
    cell = object->x >> 16;
    Func_02004834(object, ((8 - cell % 16) << 16) + object->x, object->y,
                  object->z);
    Func_0200483c(object);
    object->facing = 0xc000;
  }
  Actor_SetMovementTypeById(gAdventure.currentOwner,
                                          gData_02004c08);
  ShamanVillage_WaitFrames(12);
  ShamanVillage_SePlay(123);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004a04(work->exit);
}

void Func_02003c58(void) { Func_02003b6c(43, 40, 8, 22, 1, 2); }

void Func_02003c74(void) { Func_02003b6c(43, 42, 4, 26, 1, 2); }

void Func_02003c90(void) { Func_02003b6c(43, 42, 27, 21, 1, 2); }

void Func_02003cac(void) { Func_02003b6c(43, 40, 28, 17, 1, 2); }

void Func_02003cc8(void) { Func_02003b6c(43, 42, 10, 8, 1, 2); }
