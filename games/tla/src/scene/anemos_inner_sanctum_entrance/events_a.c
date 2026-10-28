#include "anemos_inner_sanctum_entrance.h"

extern const u8 gData_020019dc[];
extern const u8 gData_020019f4[];
extern const u8 gData_02000e20[];
extern const u8 gData_02000e5c[];
extern const u8 gData_02000ea4[];
extern u8 MsgIAmTheShadow;

const u8 *Func_0200035c(void) {
  if (gAdventure.scene == (s32)&SceneId_Ov6b1Room1)
    return gData_020019f4;
  return gData_020019dc;
}

void Func_02000388(void) {
  struct AnemosInnerSanctumEntranceParty *party;

  Func_02000cb4();
  Func_02000d44(0);
  Actor_StartRepeatedMotion(8, 2);
  FieldEvent_Wait(20);
  Func_02000d24(0x10000, 0);
  Func_02000d1c(0x10003, 1);
  Func_02000d2c(60);
  FieldEvent_Wait(60);
  Func_02000ca4((s32)&MsgIAmTheShadow, 0, 6);
  party = (struct AnemosInnerSanctumEntranceParty *)&gAdventure;
  party->battleGroup = 0x1a9f;
  party->battleRule = 2;
  Func_02000d04(103, 3);
  GameFlag_SetBit(0x201);
}

void Func_02000408(void) {
  struct Actor *object = Actor_GetById(15);

  object->motionFlags = 0;
  object->scaleX = 0x4000;
  object->scaleY = 0x4000;
  Func_02000cb4();
  Func_02000d44(0);
  Func_02000d64(151);
  Actor_SetPartAttribute(Actor_GetById(8), 7);
  Actor_SetPartAttribute(Actor_GetById(15), 7);
  FieldEvent_Wait(20);
  Func_02000d64(190);
  Func_02000cdc(15, 8);
  wait(1);
  Actor_SetMovementTypeById(8, gData_02000ea4);
  Actor_SetMovementTypeAndWaitById(15, gData_02000e20);
  Actor_SetMovementTypeAndWaitById(15, gData_02000e5c);
  FieldEvent_Wait(10);
  Func_02000cbc();
}
