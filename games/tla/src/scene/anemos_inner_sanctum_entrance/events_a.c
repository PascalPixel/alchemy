#include "anemos_inner_sanctum_entrance.h"

extern const u8 gData_020019dc[];
extern const u8 gData_020019f4[];
extern const u8 gData_02000e20[];
extern const u8 gData_02000e5c[];
extern const u8 gData_02000ea4[];
extern u8 MsgIAmTheShadow;

const u8 *Func_0200035c(void) {
  if (gPartyState.scene == (s32)&SceneId_Ov6b1Room1)
    return gData_020019f4;
  return gData_020019dc;
}

void Func_02000388(void) {
  struct AnemosInnerSanctumEntranceParty *party;

  Func_02000cb4();
  Func_02000d44(0);
  ObjectMotion_SetVariantCallback(8, 2);
  FieldEvent_WaitTicks(20);
  Func_02000d24(0x10000, 0);
  Func_02000d1c(0x10003, 1);
  Func_02000d2c(60);
  FieldEvent_WaitTicks(60);
  Func_02000ca4((s32)&MsgIAmTheShadow, 0, 6);
  party = (struct AnemosInnerSanctumEntranceParty *)&gPartyState;
  party->battleGroup = 0x1a9f;
  party->battleRule = 2;
  Func_02000d04(103, 3);
  GameFlag_SetBit(0x201);
}

void Func_02000408(void) {
  struct ObjectRuntime *object = Object_GetById(15);

  object->motionFlags = 0;
  object->scaleX = 0x4000;
  object->scaleY = 0x4000;
  Func_02000cb4();
  Func_02000d44(0);
  Func_02000d64(151);
  Object_SetPartAttribute(Object_GetById(8), 7);
  Object_SetPartAttribute(Object_GetById(15), 7);
  FieldEvent_WaitTicks(20);
  Func_02000d64(190);
  Func_02000cdc(15, 8);
  WaitTicks(1);
  ObjectMotion_EnableActionAndSetCallback(8, gData_02000ea4);
  Object_SetActionCallbackAndRefreshById(15, gData_02000e20);
  Object_SetActionCallbackAndRefreshById(15, gData_02000e5c);
  FieldEvent_WaitTicks(10);
  Func_02000cbc();
}
