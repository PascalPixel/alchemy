#include "types.h"
#include "adventure.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void FieldEvent_WaitFrames();
void Func_0200124c();
void Func_02001620();
void Func_02003d7c();
void Func_02003d84();
void Func_02003ddc();
void Func_02003de4();
void Func_02003e14();
void Func_02003e1c();
void Func_02003e34();
void Func_02003eac();
void Func_02003ef4();
void GameFlag_SetBit();
s32 GameFlag_Test();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_ResetAndSetPosition();
void ObjectMotion_ResetAndSetPositionInMode2();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
void Object_TurnToFaceEachOther();
void Object_SetModeById();
void UiText_DrawQuantityPairWithCue();
void UiText_ShowPositionedMessageAndWait();
extern u8 MsgCheckedTheTabletIt[];
extern u8 MsgWhatWentWrong[];

void Func_02001a1c(void) {
  s32 t4, t59;

  Func_02003d7c();
  Call1(Func_02003eac, 0);
  Call2(UiText_ShowPositionedMessageAndWait, (s32)&MsgCheckedTheTabletIt, 1);
  t4 = Value1(GameFlag_Test, 0x9ae);
  if (t4 == 0)
    goto L_02009a46;
  Func_02003d84();
  goto L_02009c56;
L_02009a46:
  Call1(Func_02003e14, (s32)&MsgCheckedTheTabletIt);
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), s16, 0) =
      (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  Call2(Func_02003de4, 7, gAdventure.currentOwner);
  Call1(FieldEvent_WaitFrames, 1);
  Call3(ObjectMotion_ResetAndSetPositionInMode2, gAdventure.currentOwner,
        0x100, 184);
  Call3(ObjectMotion_ResetAndSetPositionInMode2, 7, 0x112, 184);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate,
        gAdventure.currentOwner);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 7);
  Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0, 0);
  Call3(ObjectMotion_ArmCallback, 7, 0x8000, 0);
  Call1(FieldEvent_WaitFrames, 5);
  Call2(Func_02003e1c, 7, 0);
  Call2(Object_SetModeById, gAdventure.currentOwner, 3);
  Call1(FieldEvent_WaitFrames, 30);
  Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 248, 184);
  Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  Call1(FieldEvent_WaitFrames, 20);
  Call3(ObjectMotion_ArmCallback, 7, 0x8000, 0);
  Call1(FieldEvent_WaitFrames, 20);
  Call2(Object_SetModeById, 7, 3);
  Call1(FieldEvent_WaitFrames, 30);
  Call3(ObjectMotion_ArmCallback, 7, 0xc000, 0);
  Call1(FieldEvent_WaitFrames, 20);
  Call3(ObjectMotion_SetSpeedParameters, 7, 0x10000, 0x8000);
  Call3(ObjectMotion_SetPositionAndReset, 7, 0x108, 168);
  Call3(ObjectMotion_ArmCallback, 7, 0xc000, 0);
  Call1(FieldEvent_WaitFrames, 20);
  Call2(UiText_ShowPositionedMessageAndWait, ((s32)&MsgCheckedTheTabletIt + 2),
        1);
  Call2(Func_02001620, 14, 7);
  Call2(UiText_DrawQuantityPairWithCue, 7, 138);
  Call1(GameFlag_SetBit, 0x9ae);
  Call1(Func_02003ef4, 107);
  Call1(FieldEvent_WaitFrames, 30);
  Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 7, 0x4000, 0);
  Call1(FieldEvent_WaitFrames, 10);
  Value3((s32 (*)())Func_02003e34, gAdventure.currentOwner, 0x100, 0);
  Call3(Func_02003e34, 7, 0x100, 0);
  Func_0200124c();
  Call1(Func_02003ef4, 0x186);
  Call1(FieldEvent_WaitFrames, 30);
  Call1(Func_02003e14, (s32)&MsgWhatWentWrong);
  Value3((s32 (*)())Func_02003e34, 7, 0x105, 20);
  Call3(Object_TurnToFaceEachOther, gAdventure.currentOwner, 7, 0);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Func_02003e1c, 7, 0);
  Func_02003e34(7, 0x100, 20);
  Call2(Func_02003e1c, 7, 0);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Object_SetModeById, 7, 3);
  Call2(Object_SetModeById, gAdventure.currentOwner, 3);
  Call1(FieldEvent_WaitFrames, 20);
  Call3(ObjectMotion_SetSpeedParameters, 7, 0x13333, 0x9999);
  Call2(Object_SetModeById, 7, 2);
  t59 = Value1(Object_GetById, gAdventure.currentOwner);
  if (t59 == 0)
    goto L_02009c42;
  Call3(ObjectMotion_ResetAndSetPosition, 7, FIELD_AT(t59, s16, 10),
        FIELD_AT(t59, s16, 18));
L_02009c42:
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 7);
  Call3(Func_02003ddc, 7, 0, 0);
  Func_02003d84();
L_02009c56:
}
