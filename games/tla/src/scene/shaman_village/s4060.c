#include "shaman_village.h"

void Func_02003d30(void);
void Func_02003f78(void);

void Func_02004060(void) {
  if (GameFlag_Test(0x94c)) {
    ShamanVillage_MapCopy(47, 19, 38, 19, 9, 11);
    ShamanVillage_BlendStart(1, 5, 9, 1, 6, 29);
    Actor_SetPartAttribute(Actor_GetById(14), 6);
  } else if (!GameFlag_Test(0x109)) {
    s32 angle = 0x4000;

    ShamanVillage_ObjPut(15, 0xa80000, 0x2380000, angle);
    ShamanVillage_ObjPut(17, 0xd80000, 0x2180000, angle);
    ShamanVillage_ObjPut(16, 0x780000, 0x2180000, angle);
    WaitFrames(1);
  }
  Actor_SetModeById(8, 2);
  Actor_SetModeById(9, 2);
  Actor_SetModeById(10, 2);
  Actor_SetModeById(11, 2);
  Actor_SetModeById(12, 2);
  Actor_SetModeById(13, 2);
  if (!GameFlag_Test(0x109)) {
    s32 entrance = gAdventure.entrance;

    if (entrance == 11)
      Func_02003d30();
    else if (entrance == 9)
      Func_02003f78();
  }
  if (GameFlag_Test(0x9fe)) {
    Func_02004934(15, 0, 0);
    Func_02004934(17, 0, 0);
    Func_02004934(16, 0, 0);
    ShamanVillage_BlendStart(10, 12, 3, 1, 10, 13);
    ShamanVillage_BlendStart(15, 12, 3, 1, 15, 13);
    ShamanVillage_BlendStart(9, 20, 3, 1, 9, 21);
  }
}

void Func_020041b8(void) {
  struct ShamanVillageObject *object;

  Actor_SetModeById(8, 2);
  Actor_SetModeById(9, 2);
  Actor_SetModeById(10, 2);
  Actor_SetModeById(11, 2);
  Actor_SetModeById(12, 2);
  Actor_SetModeById(13, 2);
  Actor_GetById(14)->motionFlags = 0;
  if (GameFlag_Test(0x94e)) {
    Actor_SetPositionAndResetMotion(Actor_GetById(14), 0x2080000, 0x200000,
                                     0xb80000);
    WaitFrames(1);
    ShamanVillage_BlendStart(30, 11, 1, 1, 32, 11);
    WaitFrames(10);
  }
  if (GameFlag_Test(0x9e1)) {
    ShamanVillage_MapCopy(64, 0, 76, 2, 3, 3);
    ShamanVillage_BlendStart(12, 5, 3, 1, 12, 6);
  }
  if (GameFlag_Test(0x94d))
    ShamanVillage_MapCopy(48, 40, 101, 11, 1, 2);
  else
    Func_02004934(17, 0, 0);
  object = Actor_GetById(16);
  object->collisionFlags = 8;
  object->scaleX = 0xcccc;
  object->scaleY = 0xcccc;
  Actor_SetSingleChildFlags(Actor_GetById(16), 0);
}
