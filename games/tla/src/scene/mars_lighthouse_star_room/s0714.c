#include "types.h"
#include "party_state.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Camera_MoveWithinMapBounds();
s32 Event_AskYesNoWithActorAnimation();
void Event_StartScreenTransition();
void Event_WaitTransitionFrames();
void FieldEvent_WaitFrames();
void Func_02000ae4();
void Func_02000d88();
void Func_02000db8();
void Func_020041fc();
void Func_020042dc();
void Func_020042e4();
void Func_0200432c();
void Func_02004334();
void Func_0200437c();
void Func_0200438c();
void Func_02004394();
void Func_020043b4();
void Func_0200444c();
void Func_0200445c();
void Func_0200446c();
void GameFlag_SetBit();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void ObjectDispatch_SetSingleChildFlags();
void ObjectMotion_EnableActionAndSetCallback();
void ObjectMotion_SetVariantCallback();
void ObjectMotion_WaitForAnimationChange();
s32 Object_GetById();
void Object_SetActionCallbackAndRefreshById();
void Object_SetModeById();
void Object_SetPartAttribute();
void Object_SetPositionAndResetMotion();
void UiText_OpenMessageAtObject();
void WaitFrames();
extern u8 MsgMarsStar01[];
extern u8 gData_0200491c[];
extern u8 gData_02004958[];
extern u8 gData_020049a0[];
extern u8 gData_020049dc[];

void Func_02000714(void) {
  s32 t1, t2, t3, t8, t10, t12, t14, t28, t30, t32, t34, t50, t52, t87, v5, v6,
      v8;

  t1 = Value1(Object_GetById, 11);
  v5 = t1;
  t2 = Value1(Object_GetById, 15);
  v6 = t2;
  t3 = Value1(Object_GetById, 16);
  v8 = t3;
  Func_020042dc();
  Call1(Func_0200444c, 0);
  Call4(Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Func_02000d88();
  t8 = Value1(Object_GetById, 13);
  Call2(ObjectDispatch_SetSingleChildFlags, t8, 0);
  t10 = Value1(Object_GetById, 14);
  Call2(ObjectDispatch_SetSingleChildFlags, t10, 0);
  t12 = Value1(Object_GetById, 15);
  Call2(Object_SetPartAttribute, t12, 7);
  t14 = Value1(Object_GetById, 16);
  Call2(Object_SetPartAttribute, t14, 7);
  Call1(WaitFrames, 1);
  Call4(Object_SetPositionAndResetMotion, v5, FIELD_AT(v5, s32, 8), 0,
        (FIELD_AT(v5, s32, 16) + -0x80000));
  FIELD_AT(v6, u8, 85) = 0;
  FIELD_AT(v6, s32, 24) = 0x4000;
  FIELD_AT(v6, s32, 28) = 0x4000;
  FIELD_AT(v8, u8, 85) = 0;
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 10 of 343 instructions differ in
   * tla-en. */
  asm("");
  FIELD_AT(v8, s32, 24) = 0x4000;
  FIELD_AT(v8, s32, 28) = 0x4000;
  Call4(Camera_MoveWithinMapBounds, 0x2a80000, -1, 0xe40000, 0);
  Call3(Func_0200432c, gPartyState.currentOwner, 0x2a80000, 0x1080000);
  Call1(WaitFrames, 1);
  Func_020041fc();
  Call1(WaitFrames, 1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call1(Func_0200446c, 78);
  Call1(FieldEvent_WaitFrames, 20);
  Call1(Func_0200446c, 151);
  t28 = Value1(Object_GetById, 11);
  Call2(Object_SetPartAttribute, t28, 7);
  t30 = Value1(Object_GetById, 12);
  Call2(Object_SetPartAttribute, t30, 7);
  t32 = Value1(Object_GetById, 13);
  Call2(Object_SetPartAttribute, t32, 7);
  t34 = Value1(Object_GetById, 14);
  Call2(Object_SetPartAttribute, t34, 7);
  Call1(FieldEvent_WaitFrames, 20);
  Call1(Func_0200446c, 190);
  Call2(Func_02004334, 15, 11);
  Call2(Func_02004334, 16, 12);
  Call1(WaitFrames, 1);
  Call2(ObjectMotion_EnableActionAndSetCallback, 11, (s32)&gData_020049a0);
  Call2(ObjectMotion_EnableActionAndSetCallback, 12, (s32)&gData_020049dc);
  Call2(ObjectMotion_EnableActionAndSetCallback, 15, (s32)&gData_0200491c);
  Call2(Object_SetActionCallbackAndRefreshById, 16, (s32)&gData_0200491c);
  Call1(Func_02000db8, 13);
  Call1(Func_02000db8, 14);
  Call2(ObjectMotion_EnableActionAndSetCallback, 15, (s32)&gData_02004958);
  Call2(Object_SetActionCallbackAndRefreshById, 16, (s32)&gData_02004958);
  Call1(FieldEvent_WaitFrames, 10);
  t50 = Value1(Object_GetById, 13);
  Call2(Object_SetPartAttribute, t50, 0);
  t52 = Value1(Object_GetById, 14);
  Call2(Object_SetPartAttribute, t52, 0);
  Call1(FieldEvent_WaitFrames, 80);
  Call2(Motion_SetVarCbAndRefresh, 13, 2);
  Call2(Object_SetModeById, 13, 5);
  Call1(FieldEvent_WaitFrames, 20);
  Call1(Func_0200437c, (s32)&MsgMarsStar01);
  Call3(Func_0200438c, 13, 0, 80);
  Call2(Motion_SetVarCbAndRefresh, 13, 1);
  Call1(FieldEvent_WaitFrames, 20);
  Call1(Func_0200446c, 34);
  Call3(Func_0200438c, 13, 0, 20);
  Call2(Motion_SetVarCbAndRefresh, 14, 1);
  Call2(Object_SetModeById, 14, 5);
  Call1(FieldEvent_WaitFrames, 20);
  Call3(Func_0200438c, 14, 0, 20);
  Call2(Func_02004394, 14, 0);
  Call2(Motion_SetVarCbAndRefresh, 13, 2);
  Call2(Object_SetModeById, 13, 6);
  Call2(Func_02004394, 13, 0);
  Call2(Object_SetModeById, 14, 6);
  Call3(Func_0200438c, 14, 0, 20);
  Value3((s32 (*)())Func_020043b4, 13, 0x105, 40);
  Call2(Func_02004394, 13, 0);
  Func_020043b4(14, 0x105, 20);
  Call2(Func_02004394, 14, 0);
  Value3((s32 (*)())Func_020043b4, 13, 0x100, 0);
  Call3(Func_0200438c, 13, 0, 20);
  Call2(Func_02004394, 13, 0);
  Call2(Object_SetModeById, 14, 7);
  Call2(Func_02004394, 14, 0);
  Call2(Func_02004394, 13, 0);
  Call3(Func_0200438c, 14, 0, 40);
  Value3((s32 (*)())Func_020043b4, 13, 0x100, 20);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 13, 0);
  t87 = Value2(Event_AskYesNoWithActorAnimation, gPartyState.currentOwner, 0);
  if (t87 != 0)
    goto L_02008a04;
  Call1(ObjectMotion_WaitForAnimationChange, gPartyState.currentOwner);
  Call2(Object_SetModeById, 13, 7);
  Call2(Func_02004394, 13, 0);
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), s16, 0) =
      (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  goto L_02008a2c;
L_02008a04:
  FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), s16, 0) =
      (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  /* FAKEMATCH(barrier): an empty asm ends the scheduling block here, as
   * the listing orders it; without it, 2 of 343 instructions differ in
   * tla-en. */
  asm("");
  Call1(ObjectMotion_WaitForAnimationChange, gPartyState.currentOwner);
  Call2(Object_SetModeById, 13, 6);
  Call2(Func_02004394, 13, 0);
L_02008a2c:
  Call1(FieldEvent_WaitFrames, 40);
  Call2(Func_02004394, 13, 0);
  Call2(ObjectMotion_SetVariantCallback, 14, 2);
  Call2(Func_02004394, 14, 0);
  Call2(Func_02004394, 13, 0);
  Call1(Func_02000ae4, 1);
  Call2(Func_02004394, 14, 0);
  Call2(Motion_SetModeAndWaitAnimation, 13, 7);
  Call2(Func_02004394, 13, 0);
  Call2(Object_SetModeById, 13, 10);
  Call2(Object_SetModeById, 14, 9);
  Call1(GameFlag_SetBit, 0xa3b);
  Func_0200445c();
  Func_020042e4();
}
