#include "types.h"
#include "adventure.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void FieldEvent_WaitFrames();
void Func_02001730();
void Func_0200182c();
void Func_02002450();
void Func_02002520();
void Func_02002530();
void Func_02002538();
void Func_02002548();
void Func_02002578();
void Func_020025a0();
void ObjectMotion_ArmCallback();
void ObjectMotion_CommitCurrentPositionAndActivate();
void ObjectMotion_Launch();
void ObjectMotion_SetPositionAndCommit();
void ObjectMotion_SetPositionAndReset();
void ObjectMotion_SetSpeedParameters();
s32 Object_GetById();
void WaitFrames();
extern u8 MsgIfYouWantTo[];

void Func_02001ec0(void) {
  s32 t1, t24, t25, t33, t34, t42, t43, t51, t52, t63, t64, t65, t66, v5, v6,
      v7, v8;

  t1 = Value1(Object_GetById, 8);
  v6 = t1;
  Call3(ObjectMotion_ArmCallback, 8, 0x3000, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0x3000, 0);
  Call3(ObjectMotion_ArmCallback, 11, 0x5000, 0);
  Call2(Func_02002548, gAdventure.currentOwner, 0x6000);
  Call1(Func_02002520, (s32)&MsgIfYouWantTo);
  v5 = v6;
  Call2(Func_02002538, 8, 0);
  v5 = (v5 + 102);
  Func_0200182c();
  FIELD_AT(v5, s16, 0) = 1;
L_02009f28:
  Call1(WaitFrames, 1);
  v7 = v6;
  v7 = (v7 + 100);
  if (FIELD_AT(v7, s16, 0) != 218)
    goto L_02009f28;
  FIELD_AT(v5, s16, 0) = 2;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f50:
  Call1(WaitFrames, 1);
  if (FIELD_AT(v7, s16, 0) != 254)
    goto L_02009f50;
  FIELD_AT(v5, s16, 0) = 2;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f74:
  Call1(WaitFrames, 1);
  if (FIELD_AT(v7, s16, 0) != 0x102)
    goto L_02009f74;
  FIELD_AT(v5, s16, 0) = 2;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009f9c:
  Call1(WaitFrames, 1);
  if (FIELD_AT(v7, s16, 0) != 0x144)
    goto L_02009f9c;
  FIELD_AT(v5, s16, 0) = 3;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  Func_02001730();
  Call2(Func_02002548, 8, 0x5000);
  t24 = Value1(Object_GetById, 8);
  FIELD_AT(t24, u8, 90) = (254 & FIELD_AT(t24, u8, 90));
  t25 = Value1(Object_GetById, 8);
  v6 = t25;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Call3(ObjectMotion_SetSpeedParameters, 8, 0x26666, 0x13333);
  Call3(ObjectMotion_Launch, 8, 6, 0);
  Call3(ObjectMotion_SetPositionAndCommit, 8, 200, 0x29e);
  Call3(ObjectMotion_Launch, 8, 4, 0);
  Call4(Func_02002450, v6, v7, 0, v8);
  Call1(FieldEvent_WaitFrames, 5);
  Call2(Func_02002548, 10, 0x5000);
  t33 = Value1(Object_GetById, 10);
  FIELD_AT(t33, u8, 90) = (254 & FIELD_AT(t33, u8, 90));
  t34 = Value1(Object_GetById, 10);
  v6 = t34;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Call3(ObjectMotion_SetSpeedParameters, 10, 0x26666, 0x13333);
  Call3(ObjectMotion_Launch, 10, 6, 0);
  Call3(ObjectMotion_SetPositionAndCommit, 10, 200, 0x29e);
  Call3(ObjectMotion_Launch, 10, 4, 0);
  Call4(Func_02002450, v6, v7, 0, v8);
  Call1(FieldEvent_WaitFrames, 5);
  Call2(Func_02002548, 9, 0x8000);
  t42 = Value1(Object_GetById, 9);
  FIELD_AT(t42, u8, 90) = (254 & FIELD_AT(t42, u8, 90));
  t43 = Value1(Object_GetById, 9);
  v6 = t43;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Call3(ObjectMotion_SetSpeedParameters, 9, 0x26666, 0x13333);
  Call3(ObjectMotion_Launch, 9, 6, 0);
  Call3(ObjectMotion_SetPositionAndCommit, 9, 200, 0x29e);
  Call3(ObjectMotion_Launch, 9, 4, 0);
  Call4(Func_02002450, v6, v7, 0, v8);
  Call1(FieldEvent_WaitFrames, 10);
  Call2(Func_02002548, 11, 0xb000);
  t51 = Value1(Object_GetById, 11);
  v5 = (254 & FIELD_AT(t51, u8, 90));
  FIELD_AT(t51, u8, 90) = v5;
  t52 = Value1(Object_GetById, 11);
  v6 = t52;
  v8 = FIELD_AT(v6, s32, 16);
  v7 = FIELD_AT(v6, s32, 8);
  Call3(ObjectMotion_SetSpeedParameters, 11, 0x26666, 0x13333);
  Call3(ObjectMotion_Launch, 11, 6, 0);
  Call3(ObjectMotion_SetPositionAndCommit, 11, 200, 0x29e);
  Call3(ObjectMotion_Launch, 11, 4, 0);
  Call4(Func_02002450, v6, v7, 0, v8);
  Call1(ObjectMotion_CommitCurrentPositionAndActivate, 11);
  Call3(ObjectMotion_SetSpeedParameters, gAdventure.currentOwner, 0xcccc,
        0x6666);
  Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 184, 0x2ae);
  Call3(ObjectMotion_SetPositionAndReset, gAdventure.currentOwner, 186, 0x2a2);
  Call3(ObjectMotion_ArmCallback, gAdventure.currentOwner, 0xe000, 0);
  t63 = Value1(Object_GetById, 8);
  FIELD_AT(t63, u8, 90) = (FIELD_AT(t63, u8, 90) | 1);
  t64 = Value1(Object_GetById, 10);
  FIELD_AT(t64, u8, 90) = (FIELD_AT(t64, u8, 90) | 1);
  t65 = Value1(Object_GetById, 9);
  FIELD_AT(t65, u8, 90) = (FIELD_AT(t65, u8, 90) | 1);
  t66 = Value1(Object_GetById, 11);
  v5 = (1 | FIELD_AT(t66, u8, 90));
  FIELD_AT(t66, u8, 90) = v5;
  Call3(ObjectMotion_ArmCallback, 8, 0x5000, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 11, 0x8000, 20);
  Call3(Func_02002530, 8, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1(Func_020025a0, 123);
  Call1(FieldEvent_WaitFrames, 40);
  Call1(Func_02002578, 15);
}
