#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
void GameFlag_SetBit(s32 a);
void Func_02001be4(void);
void Func_02001cfc(s32 a);
void Func_02001c74(s32 a);
void FieldEvent_Wait(s32 a);
void Actor_SetPositionAndReset(s32 a, s32 b, s32 c);
void Actor_ArmCallback(s32 a, s32 b, s32 c);
void Func_02001ca4(s32 a, s32 b, s32 c);
void Func_02001c84(s32 a, s32 b, s32 c);
void Motion_SetModeAndWaitAnimation(s32 a, s32 b);
void Motion_SetVarCbAndRefresh(s32 a, s32 b);
void Actor_CommitPositionAndActivate(s32 a, s32 b, s32 c);
void Actor_StartRepeatedMotion(s32 a, s32 b);
void Func_02001cac(s32 a, s32 b);
s32 Actor_GetById(s32 a);
void FieldMessage_ShowAtActor(s32 a, s32 b);
s32 Event_AskYesNoWithActorAnimation(s32 a, s32 b);
void Func_02001c3c(s32 a, s32 b, s32 c);
void Func_02001bec(void);
extern u8 MsgAlhafra28[];
extern u8 gSequenceParentWork[];

void Func_02001958(s32 a0) {
  s32 t22;
  s32 t25;
  s32 t30;
  s32 t37;
  s32 t40;

  Call1(GameFlag_SetBit, 0x8b4);
  Func_02001be4();
  Call1(Func_02001cfc, 0);
  Call1(Func_02001c74, (s32)MsgAlhafra28);
  if (a0 == 1) {
    Call1(FieldEvent_Wait, 0xa);
    Call3(Actor_SetPositionAndReset, FIELD_AT(gSequenceParentWork, s32, 532),
          0xa8, 0x2e0);
    Call3(Actor_ArmCallback, FIELD_AT(gSequenceParentWork, s32, 532), 0, 0);
  }
  {
    s32 b = 0x102, c = 0x1e;
    /* FAKEMATCH: the native schedule sets the first argument last. */
    __asm__ volatile("" : "+l"(b), "+l"(c));
    Call3(Func_02001ca4, 0x19, b, c);
  }
  Call3(Func_02001c84, 0x19, 0, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0xa);
  Call3(Func_02001c84, 0x19, 0, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call3(Func_02001c84, 0x19, 0, 0xa);
  Call2(Motion_SetVarCbAndRefresh, 0x19, 2);
  Call1(FieldEvent_Wait, 0x14);
  Call3(Func_02001c84, 0x19, 0, 0xa);
  if (a0 != 1) {
  } else {
    Call3(Actor_CommitPositionAndActivate, 0x19, -8, 0);
    Call1(FieldEvent_Wait, 0xf);
    Call2(Actor_StartRepeatedMotion, 0x19, 3);
    Call2(Func_02001cac, 0x19, 0x102);
    Call1(FieldEvent_Wait, 0x28);
    t22 = Value1(Actor_GetById, 0x19);
    FIELD_AT((t22 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t22 + 0x5a), u8, 0));
    Call3(Actor_SetPositionAndReset, 0x19, 0xc0, 0x2e0);
    Call1(FieldEvent_Wait, 1);
    t25 = Value1(Actor_GetById, 0x19);
    FIELD_AT((t25 + 0x5a), u8, 0) = (1 | FIELD_AT((t25 + 0x5a), u8, 0));
    Call1(FieldEvent_Wait, 0xa);
    Call3(Func_02001c84, 0x19, 0, 0xa);
    {
      s32 b = 0x107, c = 0x1e;
      /* FAKEMATCH: the native schedule sets the first argument last. */
      __asm__ volatile("" : "+l"(b), "+l"(c));
      Call3(Func_02001ca4, 0x19, b, c);
    }
    Value2((s32 (*)())FieldMessage_ShowAtActor, 0x19, 0);
    t30 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
    if (t30 == 0) {
      Call1(FieldEvent_Wait, 0x1e);
      Call2(Motion_SetVarCbAndRefresh, 0x19, 2);
      Call1(FieldEvent_Wait, 0x14);
      Call3(Func_02001c84, 0x19, 0, 0xa);
      FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
          (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    } else {
      Call1(FieldEvent_Wait, 0x1e);
      FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
          (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
      Call3(Func_02001c84, 0x19, 0, 0xa);
    }
    t37 = Value1(Actor_GetById, 4);
    FIELD_AT((t37 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t37 + 0x5a), u8, 0));
    Call3(Actor_SetPositionAndReset, 4, 0x98, 0x2f0);
    Call1(FieldEvent_Wait, 1);
    t40 = Value1(Actor_GetById, 4);
    FIELD_AT((t40 + 0x5a), u8, 0) = (1 | FIELD_AT((t40 + 0x5a), u8, 0));
    Call3(Actor_ArmCallback, 4, 0xe000, 0);
    Call1(FieldEvent_Wait, 0x14);
    Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
    Call1(FieldEvent_Wait, 0xa);
    Call3(Actor_ArmCallback, 0x19, 0xe000, 0);
    Call1(FieldEvent_Wait, 0x46);
    {
      s32 b = 0x100, c = 0x32;
      /* FAKEMATCH: the native schedule sets the first argument last. */
      __asm__ volatile("" : "+l"(b), "+l"(c));
      Call3(Func_02001ca4, 0x19, b, c);
    }
    Call3(Func_02001c84, 0x19, 0, 0xa);
  }
  Call3(Actor_SetPositionAndReset, 0x19, 0xb8, 0x2d8);
  Call3(Actor_CommitPositionAndActivate, 0x19, 0, -110);
  if (a0 == 0) {
    Call1(FieldEvent_Wait, 0x32);
    Call3(Func_02001c3c, 0x19, 0x1080000, 0x2880000);
    Call3(Actor_CommitPositionAndActivate, 0x19, 0, 0x20);
    Call3(Actor_CommitPositionAndActivate, 0x19, 0x96, 0);
  }
  Call3(Func_02001c3c, 0x19, 0, 0);
  Func_02001bec();
}
