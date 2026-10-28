#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"

s32 FieldMessage_ShowAtActor();

static __inline__ void Champa_MessageOpen(s32 speaker, s32 flags) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  FieldMessage_ShowAtActor(speaker, flags);
}

#define CHAMPA_EMOTE(actor, emote, frames)                                     \
  {                                                                            \
    /* FAKEMATCH: the native loads the emote and the frames before the actor   \
     * at every */                                                             \
    /* emote of this overlay; pinned to their argument registers they are      \
     * loaded first, */                                                        \
    /* as there. Through an inline the actor comes one row early wherever the  \
     * emote takes */                                                          \
    /* two instructions, and an empty asm barrier at each call moves the       \
     * literal pools of */                                                     \
    /* a long scene. */                                                        \
    register s32 emote_ __asm__("r1") = (emote);                               \
                                                                               \
    s32 frames_ = (frames);                                                    \
    Func_02002b1c((actor), emote_, frames_);                                   \
  }
s32 GameFlag_Test();
s32 GameFlag_SetBit();
s32 Func_02002a44();
s32 Func_02002b64();
s32 Func_02002adc();
s32 Actor_SetSpeedParameters();
s32 Func_02002a9c();
s32 Func_02002bc4();
s32 Func_02002af4();
s32 Func_02002b1c();
s32 Actor_SetPositionAndReset();
s32 Camera_MoveWithinMapBounds();
s32 Func_02002b3c();
s32 FieldEvent_Wait();
s32 Func_02002b74();
s32 Actor_WaitForMoveById();
s32 Motion_SetModeAndWaitAnimation();
s32 Func_02002b04();
s32 Actor_ArmCallback();
s32 FieldMessage_ShowAtActor();
s32 Event_AskYesNoWithActorAnimation();
s32 Actor_StartRepeatedMotion();
s32 Func_02002b24();
s32 Motion_SetVarCbAndRefresh();
s32 Actor_CommitPositionAndActivate();
s32 Actor_Jump();
s32 Actor_GetById();
s32 Actor_OffsetPositionAndReset();
s32 Actor_WatchAndSetScript();
s32 Camera_FollowActor();
s32 Actor_SetModeById();
s32 Func_02000c90();
s32 Func_02002b0c();
s32 Func_02000d4c();
s32 Func_02000e04();
s32 Actor_ResetAndSetPosition();
s32 Func_02002a4c();
extern u8 MsgChampa25[];
extern u8 gMainRoutineWork[];

void Func_02000e30(void) {
  s32 t0;
  s32 t42;
  s32 t67;
  s32 t216;
  s32 t219;
  s32 t263;
  s32 t266;
  s32 t494;
  s32 t497;
  s32 t613;
  s32 t673;
  s32 t679;
  s32 t685;
  s32 t691;

  t0 = Value1(GameFlag_Test, 0x970);
  if (t0 == 0) {
    return;
  }
  Call1(GameFlag_SetBit, 0x971);
  Func_02002a44();
  Call1(Func_02002b64, 0);
  Call1(Func_02002adc, (s32)MsgChampa25);
  Call3(Actor_SetSpeedParameters, 25, 0xcccc, 0x6666);
  Call3(Actor_SetSpeedParameters, 26, 0x10000, 0x8000);
  Call3(Actor_SetSpeedParameters, 27, 0x10000, 0x8000);
  Call3(Func_02002a9c, 25, 0x1380000, 0x2080000);
  Call3(Func_02002a9c, 26, 0x1c80000, 0x2680000);
  Call3(Func_02002a9c, 27, 0xa80000, 0x2680000);
  Call1(Func_02002bc4, 0x4e);
  Call2(Func_02002af4, 0x19, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x101, 0x14)
  Call3(Actor_SetPositionAndReset, 4, 312, 616);
  Call4(Camera_MoveWithinMapBounds, 0x1380000, -1, 0x2580000, 1);
  Func_02002b3c();
  Call1(FieldEvent_Wait, 5);
  Call1(Func_02002bc4, 0x27);
  Call3(Actor_SetPositionAndReset, 25, 304, 576);
  Call1(FieldEvent_Wait, 5);
  Call4(Func_02002b74, 7, -16, 0, 0xc000);
  Call4(Func_02002b74, 5, 0, 0x10, 0xc000);
  Call4(Func_02002b74, 6, -16, 0x10, 0xc000);
  Call4(Func_02002b74, 0x1c, -32, 0, 0xd000);
  Call1(Actor_WaitForMoveById, 28);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x100, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 20);
  Call2(Func_02002b04, 7, 0);
  Call2(Func_02002af4, 7, 0);
  Call2(Func_02002b04, 28, 0);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_ArmCallback, 28, 0xd000, 0);
  Call2(Func_02002b04, 7, 0xc000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x100, 0x1e)
  Call2(Func_02002af4, 7, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x101, 0x1e)
  Champa_MessageOpen(0x2019, 0);
  t42 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t42 == 0) {
    Call1(FieldEvent_Wait, 15);
    Call2(Func_02002af4, 0x2019, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    Call1(FieldEvent_Wait, 35);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    Call2(Func_02002af4, 0x2019, 0);
  }
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call2(Func_02002b04, 4, 0x8000);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x32)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0x1e)
  Call2(Func_02002af4, 0x2005, 0);
  Call3(Actor_ArmCallback, 28, 0xd000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call2(Func_02002b04, 4, 0xc000);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002b04, 25, 0x3000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x109, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call1(FieldEvent_Wait, 5);
  FieldMessage_ShowAtActor(0x2019, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call2(Func_02002b04, 28, 0);
  t67 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t67 == 0) {
    Call1(FieldEvent_Wait, 20);
    Call2(Motion_SetModeAndWaitAnimation, 6, 3);
    Call1(FieldEvent_Wait, 10);
    Call2(Func_02002b04, 6, 0xc000);
    Call1(FieldEvent_Wait, 10);
    Call2(Func_02002af4, 6, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    Call1(FieldEvent_Wait, 40);
    Call2(Motion_SetModeAndWaitAnimation, 6, 3);
    Call1(FieldEvent_Wait, 10);
    Call2(Func_02002b04, 6, 0xc000);
    Call1(FieldEvent_Wait, 10);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    Call2(Func_02002af4, 6, 0);
  }
  Call2(Func_02002b04, 25, 0x5000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x105, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call2(Func_02002b04, 28, 0xd000);
  Call2(Motion_SetModeAndWaitAnimation, 28, 4);
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Func_02002af4, 0x1a, 0);
  Call2(Actor_StartRepeatedMotion, 4, 3);
  Call2(Func_02002b24, 4, 0x102);
  Call2(Actor_StartRepeatedMotion, 5, 3);
  Call2(Func_02002b24, 5, 0x102);
  Call2(Actor_StartRepeatedMotion, 7, 3);
  Call2(Func_02002b24, 7, 0x102);
  Call2(Actor_StartRepeatedMotion, 6, 3);
  Call2(Func_02002b24, 6, 0x102);
  Call2(Actor_StartRepeatedMotion, 28, 3);
  Call2(Func_02002b24, 0x1c, 0x102);
  Call1(FieldEvent_Wait, 40);
  Call3(Actor_ArmCallback, 25, 0x3000, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call2(Func_02002b04, 28, 0);
  Call3(Actor_SetPositionAndReset, 26, 344, 616);
  Call1(FieldEvent_Wait, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x102, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x102, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x102, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x102, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x102, 0x1e)
  Call2(Motion_SetVarCbAndRefresh, 6, 2);
  Call2(Func_02002af4, 6, 0);
  Call2(Motion_SetModeAndWaitAnimation, 26, 3);
  Call2(Func_02002af4, 0x1a, 0);
  Call2(Func_02002b04, 5, 0xc000);
  Call2(Func_02002af4, 0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Actor_StartRepeatedMotion, 4, 3);
  Call2(Func_02002b24, 4, 0x102);
  Call2(Actor_StartRepeatedMotion, 5, 3);
  Call2(Func_02002b24, 5, 0x102);
  Call2(Actor_StartRepeatedMotion, 7, 3);
  Call2(Func_02002b24, 7, 0x102);
  Call2(Actor_StartRepeatedMotion, 6, 3);
  Call2(Func_02002b24, 6, 0x102);
  Call2(Actor_StartRepeatedMotion, 28, 3);
  Call2(Func_02002b24, 0x1c, 0x102);
  Call1(FieldEvent_Wait, 40);
  Call3(Actor_ArmCallback, 25, 0x5000, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0x8000, 0);
  Call3(Actor_ArmCallback, 7, 0x8000, 0);
  Call2(Func_02002b04, 28, 0x8000);
  Call3(Actor_SetPositionAndReset, 27, 232, 616);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x3c)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x101, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Func_02002b04, 26, 0xb000);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_CommitPositionAndActivate, 27, 16, 0);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_Jump, 27, 4, 25);
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Func_02002b04, 27, 0xd000);
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 15);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call2(Func_02002b04, 6, 0);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x103, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x103, 0x32)
  Call2(Func_02002b04, 25, 0x3000);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call2(Func_02002b04, 28, 0xd000);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002b04, 25, 0x5000);
  Call1(FieldEvent_Wait, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x100, 0x1e)
  Call2(Func_02002af4, 7, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x105, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x106, 0x2a)
  Call3(Actor_CommitPositionAndActivate, 28, 0, -16);
  Call2(Func_02002b04, 28, 0xd000);
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Func_02002b04, 25, 0xd000);
  Call1(FieldEvent_Wait, 10);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0x1e)
  Call2(Func_02002af4, 0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x109, 0x1e)
  Call2(Func_02002b04, 25, 0x3000);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Func_02002b04, 25, 0x5000);
  Call1(FieldEvent_Wait, 10);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 7, 2);
  Call2(Func_02002af4, 7, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_SetSpeedParameters, 28, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 28, 8, -8);
  Call1(FieldEvent_Wait, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x108, 0x1e)
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_SetSpeedParameters, 25, 0x10000, 0x8000);
  t216 = Value1(Actor_GetById, 25);
  FIELD_AT((t216 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t216 + 0x5a), u8, 0));
  Call3(Actor_CommitPositionAndActivate, 25, -7, 7);
  Call1(FieldEvent_Wait, 1);
  t219 = Value1(Actor_GetById, 25);
  FIELD_AT((t219 + 0x5a), u8, 0) = (FIELD_AT((t219 + 0x5a), u8, 0) | 1);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002b04, 25, 0x5000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x28)
  Call2(Func_02002af4, 0x2019, 0);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x105, 0x32)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x105, 0x1e)
  Call2(Func_02002af4, 0x2005, 0);
  Call2(Actor_StartRepeatedMotion, 25, 3);
  Call2(Func_02002b24, 0x19, 0x102);
  Call1(FieldEvent_Wait, 40);
  Call2(Motion_SetVarCbAndRefresh, 26, 2);
  Call2(Func_02002b04, 26, 0xb000);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_SetSpeedParameters, 25, 0x9999, 0x4ccc);
  Call2(Func_02002b04, 25, 0);
  Call3(Actor_OffsetPositionAndReset, 25, 8, 0);
  Call2(Func_02002b04, 25, 0x3000);
  Call1(FieldEvent_Wait, 10);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x101, 0x1e)
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Func_02002b04, 25, 0x5000);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1a, 0x105, 0x1e)
  Call2(Func_02002af4, 0x1a, 0);
  Call2(Func_02002b04, 25, 0x3000);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x102, 0x1e)
  Call2(Func_02002b04, 27, 0);
  Call1(FieldEvent_Wait, 5);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call2(Func_02002b04, 7, 0);
  Call1(FieldEvent_Wait, 30);
  Call3(Actor_SetSpeedParameters, 28, 0x9999, 0x4ccc);
  Call2(Func_02002b04, 28, 0x5000);
  Call1(FieldEvent_Wait, 2);
  t263 = Value1(Actor_GetById, 28);
  FIELD_AT((t263 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t263 + 0x5a), u8, 0));
  Call3(Actor_CommitPositionAndActivate, 28, -8, 8);
  Call1(FieldEvent_Wait, 1);
  t266 = Value1(Actor_GetById, 28);
  FIELD_AT((t266 + 0x5a), u8, 0) = (1 | FIELD_AT((t266 + 0x5a), u8, 0));
  Call2(Func_02002b04, 28, 0x5000);
  Call2(Motion_SetModeAndWaitAnimation, 28, 4);
  Call2(Func_02002af4, 0x201c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1a, 0x103, 0x2a)
  Call3(Actor_Jump, 26, 6, 15);
  Call3(Actor_Jump, 26, 6, 23);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call2(Func_02002b04, 28, 0);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002b04, 26, 0x8000);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_Jump, 27, 6, 15);
  Call3(Actor_Jump, 27, 6, 23);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_SetSpeedParameters, 26, 0x1cccc, 0xe666);
  Call3(Actor_CommitPositionAndActivate, 26, -12, 0);
  Call1(FieldEvent_Wait, 5);
  Call2(Func_02002af4, 0x1a, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x102, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x102, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x102, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x102, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x102, 0x2d)
  Call3(Actor_SetSpeedParameters, 27, 0x1cccc, 0xe666);
  Call3(Actor_CommitPositionAndActivate, 27, 12, 0);
  Call1(FieldEvent_Wait, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x108, 0x1e)
  Call2(Func_02002af4, 0x1b, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x100, 0x14)
  Call2(Func_02002b04, 6, 0x8000);
  Call2(Func_02002af4, 0x2006, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 26, 0xb000, 0);
  Call3(Actor_ArmCallback, 27, 0xd000, 0);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call2(Func_02002b04, 28, 0xd000);
  Call1(FieldEvent_Wait, 10);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1a, 0x107, 0x1e)
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 5, 0xe000, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call3(Actor_ArmCallback, 25, 0x3000, 0);
  Call2(Func_02002b04, 28, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x103, 0x2a)
  Call3(Actor_Jump, 27, 6, 15);
  Call3(Actor_Jump, 27, 6, 23);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 7, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0xa000, 0);
  Call3(Actor_ArmCallback, 6, 0xa000, 0);
  Call3(Actor_ArmCallback, 25, 0x5000, 0);
  Call2(Func_02002b04, 28, 0x5000);
  Call1(FieldEvent_Wait, 20);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 26, 2);
  Call2(Func_02002b04, 26, 0xb000);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 5, 0xe000, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call3(Actor_ArmCallback, 25, 0x3000, 0);
  Call2(Func_02002b04, 28, 0);
  Call1(FieldEvent_Wait, 20);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x109, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 27, 2);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 7, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0xa000, 0);
  Call3(Actor_ArmCallback, 6, 0xa000, 0);
  Call3(Actor_ArmCallback, 25, 0x5000, 0);
  Call2(Func_02002b04, 28, 0x5000);
  Call1(FieldEvent_Wait, 15);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 25);
  Call2(Motion_SetVarCbAndRefresh, 26, 2);
  Call2(Func_02002af4, 0x1a, 0);
  Call2(Func_02002b04, 25, 0x3000);
  Call2(Func_02002af4, 0x2019, 0);
  Call1(FieldEvent_Wait, 10);
  Call2(Motion_SetVarCbAndRefresh, 27, 2);
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Func_02002b04, 25, 0x5000);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call2(Func_02002b04, 28, 0xd000);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x101, 0x1e)
  Call2(Func_02002af4, 0x2005, 0);
  Call2(Func_02002b04, 25, 0x3000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Func_02002b04, 25, 0x5000);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 7, 2);
  Call2(Func_02002af4, 7, 0);
  Call2(Func_02002b04, 25, 0xd000);
  Call1(FieldEvent_Wait, 15);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetModeAndWaitAnimation, 26, 3);
  Call2(Func_02002af4, 0x1a, 0);
  Call2(Func_02002b04, 25, 0x3000);
  Call1(FieldEvent_Wait, 10);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 20);
  Call2(Motion_SetVarCbAndRefresh, 27, 2);
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Func_02002b04, 27, 0);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 7, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0x8000, 0);
  Call2(Func_02002b04, 28, 0x8000);
  Call1(FieldEvent_Wait, 20);
  Call2(Actor_WatchAndSetScript, 4, 27);
  Call2(Actor_WatchAndSetScript, 7, 27);
  Call2(Actor_WatchAndSetScript, 5, 27);
  Call2(Actor_WatchAndSetScript, 6, 27);
  Call2(Actor_WatchAndSetScript, 28, 27);
  Call3(Actor_SetSpeedParameters, 27, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 27, 20, 0);
  Call3(Actor_CommitPositionAndActivate, 27, 2, 32);
  Call3(Actor_CommitPositionAndActivate, 27, 32, 32);
  Call3(Actor_CommitPositionAndActivate, 27, 0, 32);
  Call2(Func_02002b04, 26, 0x8000);
  Call2(Motion_SetModeAndWaitAnimation, 26, 3);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call2(Func_02002b04, 28, 0);
  Call1(FieldEvent_Wait, 20);
  Call2(Actor_WatchAndSetScript, 4, 26);
  Call2(Actor_WatchAndSetScript, 7, 26);
  Call2(Actor_WatchAndSetScript, 5, 26);
  Call2(Actor_WatchAndSetScript, 6, 26);
  Call2(Actor_WatchAndSetScript, 28, 26);
  Call3(Actor_SetSpeedParameters, 26, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 26, 0, 32);
  Call3(Actor_CommitPositionAndActivate, 26, -20, 24);
  Call3(Actor_CommitPositionAndActivate, 26, 0, 48);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call2(Func_02002b04, 28, 0xd000);
  Call1(FieldEvent_Wait, 20);
  Call3(Func_02002a9c, 26, 0, 0);
  Call3(Func_02002a9c, 27, 0, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Actor_WatchAndSetScript, 4, 25);
  Call2(Actor_WatchAndSetScript, 7, 25);
  Call2(Actor_WatchAndSetScript, 5, 25);
  Call2(Actor_WatchAndSetScript, 6, 25);
  Call2(Actor_WatchAndSetScript, 28, 25);
  Call3(Actor_SetSpeedParameters, 25, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 25, 22, 24);
  Call2(Camera_FollowActor, 25, 1);
  Call3(Actor_CommitPositionAndActivate, 25, 0, 32);
  Call3(Actor_CommitPositionAndActivate, 25, -16, 16);
  Call3(Actor_CommitPositionAndActivate, 25, 0, 8);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x100, 0x28)
  Call2(Func_02002b04, 25, 0xb000);
  Call4(Camera_MoveWithinMapBounds, 0x1300000, -1, 0x2880000, 1);
  Func_02002b3c();
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 28, 2);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_SetSpeedParameters, 28, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 28, 0, 32);
  Call2(Func_02002b04, 28, 0x3000);
  Call1(FieldEvent_Wait, 10);
  Call2(Motion_SetModeAndWaitAnimation, 28, 3);
  Call2(Func_02002af4, 0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x1e)
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x107, 0x1e)
  Call2(Func_02002af4, 0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Actor_StartRepeatedMotion, 28, 3);
  Call2(Func_02002b24, 0x1c, 0x102);
  Call1(FieldEvent_Wait, 40);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call2(Func_02002b04, 7, 0);
  Call1(FieldEvent_Wait, 40);
  Call3(Actor_ArmCallback, 4, 0x4000, 0);
  Call3(Actor_ArmCallback, 5, 0x4000, 0);
  Call3(Actor_ArmCallback, 6, 0x4000, 0);
  Call2(Func_02002b04, 7, 0x4000);
  Call1(FieldEvent_Wait, 25);
  Call3(Actor_SetSpeedParameters, 28, 0x6666, 0x3333);
  t494 = Value1(Actor_GetById, 28);
  FIELD_AT((t494 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t494 + 0x5a), u8, 0));
  Call3(Actor_CommitPositionAndActivate, 28, 0, -24);
  Call1(FieldEvent_Wait, 1);
  t497 = Value1(Actor_GetById, 28);
  FIELD_AT((t497 + 0x5a), u8, 0) = (1 | FIELD_AT((t497 + 0x5a), u8, 0));
  Call1(FieldEvent_Wait, 10);
  Call2(Actor_SetModeById, 28, 8);
  Call1(FieldEvent_Wait, 30);
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Func_02002b04, 6, 0x4000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 25);
  Call2(Motion_SetVarCbAndRefresh, 5, 2);
  Call2(Func_02002af4, 0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x105, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call2(Func_02002b04, 7, 0);
  Call1(FieldEvent_Wait, 40);
  Call3(Actor_ArmCallback, 4, 0x4000, 0);
  Call2(Func_02002b04, 7, 0x4000);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x101, 0x1e)
  Call2(Func_02002af4, 7, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 3);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetModeAndWaitAnimation, 25, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 6, 2);
  Call2(Func_02002af4, 6, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Func_02002b04, 25, 0x5000);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2019, 0);
  Call1(Func_02002bc4, 0x4e);
  Call3(Actor_CommitPositionAndActivate, 25, 0, 80);
  Call3(Func_02002a9c, 25, 0, 0);
  Call1(FieldEvent_Wait, 30);
  Call4(Camera_MoveWithinMapBounds, 0x1300000, -1, 0x2700000, 1);
  Func_02002b3c();
  Call1(FieldEvent_Wait, 10);
  Call1(Func_02002bc4, 3);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0x1e)
  Call2(Func_02002af4, 0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x103, 0x1e)
  Call2(Func_02002af4, 6, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x109, 0x1e)
  Call2(Func_02002af4, 7, 0);
  Call2(Func_02002b04, 6, 0xc000);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Motion_SetVarCbAndRefresh, 7, 2);
  Call2(Func_02002af4, 7, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x100, 0x1e)
  Call3(Actor_ArmCallback, 4, 0x6000, 0);
  Call2(Func_02002b04, 5, 0xa000);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x100, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Actor_SetModeById, 28, 1);
  Call1(FieldEvent_Wait, 10);
  Call3(Actor_SetSpeedParameters, 28, 0xcccc, 0x6666);
  Call3(Actor_CommitPositionAndActivate, 28, 0, 16);
  Call2(Func_02002b04, 28, 0);
  Call1(FieldEvent_Wait, 5);
  Call2(Motion_SetModeAndWaitAnimation, 28, 4);
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0x8000, 0);
  Call2(Func_02002b04, 7, 0x8000);
  Call1(FieldEvent_Wait, 20);
  Call2(Motion_SetModeAndWaitAnimation, 28, 3);
  Call2(Func_02002af4, 0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x100, 0x1e)
  Call2(Func_02002af4, 0x2007, 0);
  Call2(Motion_SetModeAndWaitAnimation, 28, 4);
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Actor_StartRepeatedMotion, 7, 3);
  Call2(Func_02002b24, 7, 0x102);
  Call1(FieldEvent_Wait, 45);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x101, 0x1e)
  Call2(Func_02002af4, 0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x1e)
  Call2(Func_02002af4, 0x2006, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call2(Func_02002b04, 6, 1);
  Call1(FieldEvent_Wait, 25);
  Call2(Func_02000c90, 5, 6);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x28)
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call2(Func_02002b04, 6, 0x8000);
  Call1(FieldEvent_Wait, 10);
  Call2(Actor_SetModeById, 5, 4);
  Call2(Motion_SetModeAndWaitAnimation, 6, 4);
  Call11(Func_02002b0c, 5, 5, 5, 0x12, 8, 6, 6, 0xe, 8, 9, 0);
  Call2(Motion_SetModeAndWaitAnimation, 28, 3);
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Actor_StartRepeatedMotion, 4, 3);
  Call2(Func_02002b24, 4, 0x102);
  Call1(FieldEvent_Wait, 40);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call2(Func_02002b04, 6, 0xe000);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x1e)
  Call11(Func_02000d4c, 5, 8, 5, 0x12, 8, 6, 8, 0xe, 8, 9, 0);
  Call2(Func_02002b04, 4, 0x6000);
  t613 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t613 == 0) {
    Call2(Func_02000e04, 5, 6);
    Call1(FieldEvent_Wait, 15);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * CHAMPA_EMOTE. */
    CHAMPA_EMOTE(0x1c, 0x102, 0x1e)
    Call2(Func_02002af4, 0x1c, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    Call2(Func_02000e04, 5, 6);
    Call1(FieldEvent_Wait, 35);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * CHAMPA_EMOTE. */
    CHAMPA_EMOTE(0x1c, 0x105, 0x1e)
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    Call2(Func_02002af4, 0x1c, 0);
  }
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call2(Func_02002b04, 6, 0x8000);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x2d)
  Call2(Motion_SetModeAndWaitAnimation, 28, 4);
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_Jump, 5, 6, 15);
  Call3(Actor_Jump, 5, 6, 23);
  Call2(Func_02002af4, 0x2005, 0);
  Call2(Motion_SetModeAndWaitAnimation, 28, 3);
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_Jump, 6, 6, 15);
  Call3(Actor_Jump, 6, 6, 23);
  Call2(Func_02002af4, 0x2006, 0);
  Call2(Motion_SetModeAndWaitAnimation, 28, 3);
  Call2(Func_02002af4, 0x1c, 0);
  Call1(FieldEvent_Wait, 10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x28)
  Call3(Actor_CommitPositionAndActivate, 28, 0, -8);
  Call2(Func_02002b04, 28, 0);
  Call1(FieldEvent_Wait, 5);
  Call2(Func_02002af4, 0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2007, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x102, 0x1e)
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Motion_SetModeAndWaitAnimation, 7, 3);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2007, 0);
  Call1(FieldEvent_Wait, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x2d)
  Call2(Motion_SetModeAndWaitAnimation, 7, 3);
  Call1(FieldEvent_Wait, 10);
  Call2(Func_02002af4, 0x2007, 0);
  Call2(Motion_SetVarCbAndRefresh, 28, 2);
  Call1(FieldEvent_Wait, 20);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call2(Func_02002b04, 7, 0);
  Call1(FieldEvent_Wait, 20);
  Call2(Motion_SetModeAndWaitAnimation, 4, 3);
  Call1(FieldEvent_Wait, 15);
  Call2(Actor_SetModeById, 5, 3);
  Call2(Actor_SetModeById, 6, 3);
  Call2(Actor_SetModeById, 7, 3);
  Call2(Motion_SetModeAndWaitAnimation, 28, 3);
  Call1(FieldEvent_Wait, 20);
  Call3(Actor_SetSpeedParameters, 5, 0x13333, 0x9999);
  Call2(Actor_SetModeById, 5, 2);
  t673 = Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532));
  if (t673 != 0) {
    Call3(Actor_ResetAndSetPosition, 5, FIELD_AT(t673, s16, 10),
          FIELD_AT(t673, s16, 18));
  }
  Call1(Actor_WaitForMoveById, 5);
  Call3(Func_02002a9c, 5, 0, 0);
  Call3(Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
  Call2(Actor_SetModeById, 6, 2);
  t679 = Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532));
  if (t679 != 0) {
    Call3(Actor_ResetAndSetPosition, 6, FIELD_AT(t679, s16, 10),
          FIELD_AT(t679, s16, 18));
  }
  Call1(Actor_WaitForMoveById, 6);
  Call3(Func_02002a9c, 6, 0, 0);
  Call3(Actor_SetSpeedParameters, 7, 0x13333, 0x9999);
  Call2(Actor_SetModeById, 7, 2);
  t685 = Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532));
  if (t685 != 0) {
    Call3(Actor_ResetAndSetPosition, 7, FIELD_AT(t685, s16, 10),
          FIELD_AT(t685, s16, 18));
  }
  Call1(Actor_WaitForMoveById, 7);
  Call3(Func_02002a9c, 7, 0, 0);
  Call3(Actor_SetSpeedParameters, 28, 0x13333, 0x9999);
  Call2(Actor_SetModeById, 28, 2);
  t691 = Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532));
  if (t691 != 0) {
    Call3(Actor_ResetAndSetPosition, 28, FIELD_AT(t691, s16, 10),
          FIELD_AT(t691, s16, 18));
  }
  Call1(Actor_WaitForMoveById, 28);
  Call3(Func_02002a9c, 28, 0, 0);
  Func_02002a4c();
}
