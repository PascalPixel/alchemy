#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
s32 GameFlag_Test();
s32 GameFlag_SetBit();
s32 Func_02002a44();
s32 Func_02002b64();
s32 Func_02002adc();
s32 Func_02002aa4();
s32 Func_02002aec();
s32 ObjectMotion_SetPositionAndReset();
s32 ObjectMotion_ArmCallback();
s32 FieldEvent_WaitFrames();
s32 Camera_FollowActor();
s32 Func_02002b3c();
s32 Func_02002b1c();
s32 UiText_OpenMessageAtObject();
s32 Event_AskYesNoWithActorAnimation();
s32 Func_02002b74();
s32 ObjectMotion_CommitCurrentPositionAndActivate();
s32 Motion_SetVarCbAndRefresh();
s32 Motion_SetModeAndWaitAnimation();
s32 Func_02002b04();
s32 Object_LinkPair();
s32 ObjectMotion_Launch();
void Func_0200060c(void);
s32 Object_SetModeById();
s32 ObjectMotion_SetSpeedParameters();
s32 ObjectMotion_OffsetPositionAndReset();
s32 Object_GetById();
s32 ObjectMotion_ResetAndSetPosition();
s32 Func_02002a9c();
s32 Func_02002a4c();
extern u8 MsgChampa52[];

void Func_02000638(void) {
  s32 t0;
#if !defined(TLA_EDITION_JA)
  s32 t1;
#endif
  s32 t17;
  s32 t106;

  t0 = Value1(GameFlag_Test, 0x822);
  if (t0 == 0) {
  } else {
#if !defined(TLA_EDITION_JA)
    // The Japanese edition has no flag 0 test.
    t1 = Value1(GameFlag_Test, 0);
    if (t1 != 0) {
    } else
#endif
    {
      Call1(GameFlag_SetBit, 0xa75);
      Func_02002a44();
      Call1(Func_02002b64, 0);
      Call1(Func_02002adc, (s32)MsgChampa52);
      {
        /* FAKEMATCH: the local starts the facing before the other arguments, as
         * the native has (one reordered row as a plain argument). */
        s32 angle = 0xc000;

        Call4(Func_02002aa4, 0x1d, 0x13a0000, 0x2a70000, angle);
      }
      Call3(Func_02002aec, 0x1d, 0, 5);
      Call3(ObjectMotion_SetPositionAndReset, 4, 0x13a, 0x244);
      Call3(ObjectMotion_ArmCallback, 4, 0x4000, 0);
      Call1(FieldEvent_WaitFrames, 0xa);
      Call2(Camera_FollowActor, 0x1d, 1);
      Func_02002b3c();
      Call1(FieldEvent_WaitFrames, 0x28);
      Call3(ObjectMotion_SetPositionAndReset, 0x1d, 0x13a, 0x270);
      Func_02002b1c(0x1d, 0x101, 0x1e);
      UiText_OpenMessageAtObject(0x1d, 0);
      t17 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
      if (t17 == 0) {
        Call3(Func_02002aec, 0x1d, 0, 5);
        FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
            (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
      } else {
        FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
            (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
        Call3(Func_02002aec, 0x1d, 0, 5);
      }
      Func_02002b1c(0x1d, 0x109, 0x1e);
      Call4(Func_02002b74, 0x1e, 0x10, 0, 0x4000);
      Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0x1e);
      Call1(FieldEvent_WaitFrames, 0x14);
      {
        /* FAKEMATCH: the native loads the emote and the frames before the
         * actor; pinned to their argument registers they are loaded first (the
         * actor comes one row early through the inline). */
        register s32 emote asm("r1") = 0x101;
        /* FAKEMATCH: the frames, pinned with the emote above. */
        register s32 frames asm("r2") = 0x1e;

        Func_02002b1c(0x1e, emote, frames);
      }
      Call3(Func_02002aec, 0x1e, 0, 5);
      Call2(Motion_SetVarCbAndRefresh, 0x1d, 2);
      Call1(FieldEvent_WaitFrames, 5);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 0x1e, 3);
      Call1(FieldEvent_WaitFrames, 0xa);
      Call2(Func_02002b04, 0x1d, 0x4000);
      Call1(FieldEvent_WaitFrames, 0x14);
      Call2(Func_02002b04, 0x1d, 0xc000);
      Call1(FieldEvent_WaitFrames, 0x14);
      Call2(Func_02002b04, 0x1d, 0x4000);
      Call1(FieldEvent_WaitFrames, 0x14);
      Call2(Func_02002b04, 0x1d, 0xc000);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Call3(Object_LinkPair, 0x1e, 4, 0);
      Call1(FieldEvent_WaitFrames, 0x28);
      Call3(ObjectMotion_ArmCallback, 0x1e, 0x4000, 0);
      Call3(ObjectMotion_ArmCallback, 4, 0x4000, 0);
      Call1(FieldEvent_WaitFrames, 0x14);
      Call2(Motion_SetModeAndWaitAnimation, 0x1e, 4);
      Call3(Func_02002aec, 0x1e, 0, 5);
      Func_02002b1c(0x1d, 0x107, 0x1e);
      Call3(ObjectMotion_Launch, 0x1d, 2, 0);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Func_02002b1c(0x1e, 0x101, 0x1e);
      Call3(Func_02002aec, 0x1e, 0, 5);
      Call2(Motion_SetVarCbAndRefresh, 0x1d, 2);
      Call1(FieldEvent_WaitFrames, 5);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Call3(Func_02002aec, 0x1d, 0, 5);
      {
        s32 b = 0x108, c = 0x1e;
        /* FAKEMATCH: the native schedule sets the first argument last. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Call3(Func_02002b1c, 0x1e, b, c);
      }
      Call3(Func_02002aec, 0x1e, 0, 5);
      Func_02002b1c(0x1d, 0x102, 0x14);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 0x1e, 3);
      Call1(FieldEvent_WaitFrames, 0xa);
      Call3(Func_02002aec, 0x1e, 0, 5);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Call2(Func_02002b04, 0x1d, 0x4000);
      Call1(FieldEvent_WaitFrames, 0xa);
      {
        s32 b = 0x108, c = 0x1e;
        /* FAKEMATCH: the native schedule sets the first argument last. */
        __asm__ volatile("" : "+l"(b), "+l"(c));
        Call3(Func_02002b1c, 0x1e, b, c);
      }
      Call3(Func_02002aec, 0x1e, 0, 5);
      Call2(Func_02002b04, 0x1d, 0xc000);
      Call1(FieldEvent_WaitFrames, 0xa);
      Func_02002b1c(0x1d, 0x108, 0x1e);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 0x1e, 3);
      Call1(FieldEvent_WaitFrames, 0xa);
      Call2(Motion_SetVarCbAndRefresh, 0x1d, 2);
      Call1(FieldEvent_WaitFrames, 5);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Func_0200060c();
      Call3(Object_LinkPair, 0x1e, 4, 0);
      Call1(FieldEvent_WaitFrames, 0x28);
      Call3(ObjectMotion_ArmCallback, 0x1e, 0x4000, 0);
      Call3(ObjectMotion_ArmCallback, 4, 0x4000, 0);
      Call1(FieldEvent_WaitFrames, 0x14);
      Call3(Func_02002aec, 0x1e, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 0x1d, 3);
      Call1(FieldEvent_WaitFrames, 0xa);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Func_02002b1c(0x1e, 0x100, 0x1e);
      Call3(Func_02002aec, 0x1e, 0, 5);
      Call2(Motion_SetModeAndWaitAnimation, 0x1d, 3);
      Call1(FieldEvent_WaitFrames, 0xa);
      Call3(Func_02002aec, 0x1d, 0, 5);
      Call2(Object_SetModeById, 4, 3);
      Call2(Motion_SetModeAndWaitAnimation, 0x1e, 3);
      Call1(FieldEvent_WaitFrames, 0xa);
      Call2(Motion_SetModeAndWaitAnimation, 0x1d, 3);
      Call1(FieldEvent_WaitFrames, 0xa);
      Call2(Camera_FollowActor, 4, 1);
      Call3(ObjectMotion_SetSpeedParameters, 0x1d, 0x13333, 0x9999);
      Call3(ObjectMotion_OffsetPositionAndReset, 0x1d, 0, 0x64);
      Call1(FieldEvent_WaitFrames, 0xa);
      Call3(Object_LinkPair, 4, 0x1e, 0);
      Call1(FieldEvent_WaitFrames, 0x1e);
      Call2(Object_SetModeById, 0x1e, 3);
      Call2(Motion_SetModeAndWaitAnimation, 4, 3);
      Call1(FieldEvent_WaitFrames, 0x1e);
      Call2(Object_SetModeById, 0x1e, 2);
      t106 = Value1(Object_GetById, 4);
      if (t106 != 0) {
        Call3(ObjectMotion_ResetAndSetPosition, 0x1e, FIELD_AT(t106, s16, 10),
              FIELD_AT(t106, s16, 18));
      }
      Call1(ObjectMotion_CommitCurrentPositionAndActivate, 0x1e);
      Call3(Func_02002a9c, 0x1e, 0, 0);
      Call3(Func_02002a9c, 0x1d, 0, 0);
      Func_02002a4c();
    }
  }
}
