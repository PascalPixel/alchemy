#include "types.h"
#include "party_state.h"
#include "call.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 Event_AskYesNoWithActorAnimation();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void FieldEvent_WaitFrames();
void Func_02001730();
void Func_0200182c();
void Func_02001ec0();
void Func_02002520();
void Func_02002530();
void Func_02002538();
void Func_02002548();
void Func_02002578();
void Func_020025a0();
void ObjectMotion_ArmCallback();
void ObjectMotion_Launch();
s32 Object_GetById();
void UiText_OpenMessageAtObject();
void WaitFrames();
extern u8 MsgIfYouWant[];

void Func_02001d14(void) {
  s32 t1, t33, v5, v6, v7;

  t1 = Value1(Object_GetById, 8);
  v7 = t1;
  Call3(ObjectMotion_ArmCallback, 8, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0x8000, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0xb000, 0);
  Call3(ObjectMotion_ArmCallback, 11, 0xb000, 0);
  Call2(Func_02002548, gPartyState.currentOwner, 0);
  Call1(Func_02002520, (s32)&MsgIfYouWant);
  v5 = v7;
  Call2(Func_02002538, 8, 0);
  v5 = (v5 + 102);
  Func_0200182c();
  FIELD_AT(v5, s16, 0) = 1;
L_02009d78:
  Call1(WaitFrames, 1);
  v6 = v7;
  v6 = (v6 + 100);
  if (FIELD_AT(v6, s16, 0) != 114)
    goto L_02009d78;
  FIELD_AT(v5, s16, 0) = 2;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009da0:
  Call1(WaitFrames, 1);
  if (FIELD_AT(v6, s16, 0) != 150)
    goto L_02009da0;
  FIELD_AT(v5, s16, 0) = 2;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009dc4:
  Call1(WaitFrames, 1);
  if (FIELD_AT(v6, s16, 0) != 154)
    goto L_02009dc4;
  FIELD_AT(v5, s16, 0) = 2;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  FIELD_AT(v5, s16, 0) = 1;
L_02009de8:
  Call1(WaitFrames, 1);
  if (FIELD_AT(v6, s16, 0) != 216)
    goto L_02009de8;
  FIELD_AT(v5, s16, 0) = 3;
  Call1(WaitFrames, 1);
  Call2(Func_02002538, 8, 0);
  Func_02001730();
  Call3(ObjectMotion_Launch, 8, 4, 0);
  Call3(ObjectMotion_Launch, 10, 4, 0);
  Call3(ObjectMotion_Launch, 9, 4, 0);
  Call3(ObjectMotion_Launch, 11, 4, 40);
  v5 = (s32)&gPartyState;
  v5 = (v5 + 0x214);
  Call3(ObjectMotion_ArmCallback, FIELD_AT(v5, s32, 0), 0x4000, 0);
  Call3(ObjectMotion_ArmCallback, 8, 0xd000, 0);
  Call3(ObjectMotion_ArmCallback, 10, 0xd000, 0);
  Call3(ObjectMotion_ArmCallback, 9, 0xd000, 0);
  Call2(Func_02002548, 11, 0xd000);
  Value2((s32 (*)())UiText_OpenMessageAtObject, 8, 0);
  t33 = Value2(Event_AskYesNoWithActorAnimation, FIELD_AT(v5, s32, 0), 0);
  if (t33 != 0)
    goto L_02009e90;
  Func_02001ec0();
  goto L_02009eb4;
L_02009e90:
  Call3(Func_02002530, 8, 0, 20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call1(Func_020025a0, 123);
  Call1(FieldEvent_WaitFrames, 40);
  Call1(Func_02002578, 15);
L_02009eb4:
}
