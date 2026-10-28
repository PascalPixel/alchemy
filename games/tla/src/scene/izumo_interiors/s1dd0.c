#include "types.h"
#include "adventure.h"
#include "call.h"

s32 Event_AskYesNoWithActorAnimation();
void FieldEvent_WaitFrames();
void Func_020021c4();
void Func_020021cc();
void Func_02002214();
void Func_02002244();
void Func_02002254();
void Func_0200226c();
void Func_020022bc();
void Func_020022c4();
void Func_020022e4();
void Func_020022ec();
void Func_020022f4();
void Func_020022fc();
void Func_0200231c();
void GameFlag_SetBit();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_Launch();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_SetAngleToward();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetVariantCallback();
s32 Object_GetById();
void Object_LinkPair();
void Object_SetModeById();
void PartyInventory_Remove();
void UiText_OpenMessageAtObject();
extern u8 MsgAgainYouHave[];
extern u8 MsgAhCouldIt[];

void Func_02001dd0(void) {
  s32 t9, t21, t31;

  Func_020021c4();
  Call1(Func_020022bc, 0);
  Call1(Func_02002244, (s32)&MsgAhCouldIt);
  Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 144, 0x218);
  Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xc000, 0);
  Call1(FieldEvent_WaitFrames, 15);
  Value3((s32 (*)())Func_0200226c, 15, 0x100, 60);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 15, 0);
  t9 = Value2(Event_AskYesNoWithActorAnimation, gAdventure.currentOwner, 0);
  if (t9 != 0)
    goto L_02009e3a;
  Call1(Func_02002244, ((s32)&MsgAhCouldIt + 1));
  Call2(Func_02002254, 15, 0);
  goto L_02009e48;
L_02009e3a:
  Call1(Func_02002244, ((s32)&MsgAhCouldIt + 2));
  Call2(Func_02002254, 15, 0);
L_02009e48:
  Call3(Func_0200226c, 15, 0x109, 60);
  Call1(Func_02002244, (s32)&MsgAgainYouHave);
  Call2(Func_02002254, 15, 0);
  Value3((s32 (*)())Func_0200226c, 15, 0x105, 120);
  Func_0200226c(15, 0x106, 60);
  Call2(Func_02002254, 15, 0);
  Func_020022e4();
  t21 = Value1(Object_GetById, 15);
  Call2(Func_020022fc, t21, 2);
  Call1(Func_0200231c, 201);
  Call1(FieldEvent_WaitFrames, 60);
  Call3(Func_02002214, 24, 0x900000, 0x2080000);
  Call3(ObjectMotion_Launch, 24, 6, 0);
  Call3(ObjectMotion_ResetAndSetPosition, 24, 160, 0x208);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 24);
  Call3(ObjectMotion_ArmCallback, 24, 0x4000, 0);
  Call1(FieldEvent_WaitFrames, 60);
  t31 = Value1(Object_GetById, 15);
  Call2(Func_020022fc, t31, 0);
  Func_020022f4();
  Func_020022ec();
  Call1(FieldEvent_WaitFrames, 30);
  Call1(FieldEvent_WaitFrames, 30);
  Call3(ObjectMotion_SetAngleToward, 15, 24, 0);
  Call1(FieldEvent_WaitFrames, 30);
  Call2(Func_02002254, 15, 0);
  Call3(ObjectMotion_SetAngleToward, 15, gAdventure.currentOwner, 0);
  Call1(FieldEvent_WaitFrames, 60);
  Call2(Func_02002254, 15, 0);
  Call1(FieldEvent_WaitFrames, 30);
  Call2(Object_SetModeById, 15, 4);
  Call1(FieldEvent_WaitFrames, 60);
  Call2(Func_02002254, 15, 0);
  Call3(Object_LinkPair, 15, 24, 0);
  Call2(Func_02002254, 15, 0);
  Call1(FieldEvent_WaitFrames, 60);
  Call3(ObjectMotion_Launch, 24, 4, 20);
  Call1(FieldEvent_WaitFrames, 60);
  Call3(ObjectMotion_SetAngleToward, 15, gAdventure.currentOwner, 0);
  Call3(ObjectMotion_SetAngleToward, 24, gAdventure.currentOwner, 0);
  Call1(FieldEvent_WaitFrames, 60);
  Call2(Func_02002254, 15, 0);
  Call3(ObjectMotion_Launch, 24, 2, 20);
  Call3(Func_020022c4, 24, 2, 11);
  Func_020021cc();
  Func_020021c4();
  Call1(Func_020022bc, 0);
  Call3(ObjectMotion_SetAngleToward, gAdventure.currentOwner, 15, 0);
  Call1(FieldEvent_WaitFrames, 60);
  Call2(ObjectMotion_SetVariantCallback, 15, 2);
  Call1(FieldEvent_WaitFrames, 30);
  Call2(Func_02002254, 15, 0);
  Call1(FieldEvent_WaitFrames, 60);
  Call1(GameFlag_SetBit, 0x92c);
  Call1(PartyInventory_Remove, 0x1c3);
  Func_020021cc();
}
