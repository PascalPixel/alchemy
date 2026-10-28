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
  Call3(Actor_SetSpeedParameters, 0x19, 0xcccc, 0x6666);
  Call3(Actor_SetSpeedParameters, 0x1a, 0x10000, 0x8000);
  Call3(Actor_SetSpeedParameters, 0x1b, 0x10000, 0x8000);
  Call3(Func_02002a9c, 0x19, 0x1380000, 0x2080000);
  Call3(Func_02002a9c, 0x1a, 0x1c80000, 0x2680000);
  Call3(Func_02002a9c, 0x1b, 0xa80000, 0x2680000);
  Call1(Func_02002bc4, 0x4e);
  Call2(Func_02002af4, 0x19, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x101, 0x14)
  Call3(Actor_SetPositionAndReset, 4, 0x138, 0x268);
  Call4(Camera_MoveWithinMapBounds, 0x1380000, -1, 0x2580000, 1);
  Func_02002b3c();
  Call1(FieldEvent_Wait, 5);
  Call1(Func_02002bc4, 0x27);
  Call3(Actor_SetPositionAndReset, 0x19, 0x130, 0x240);
  Call1(FieldEvent_Wait, 5);
  Call4(Func_02002b74, 7, -16, 0, 0xc000);
  Call4(Func_02002b74, 5, 0, 0x10, 0xc000);
  Call4(Func_02002b74, 6, -16, 0x10, 0xc000);
  Call4(Func_02002b74, 0x1c, -32, 0, 0xd000);
  Call1(Actor_WaitForMoveById, 0x1c);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x100, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Func_02002b04, 7, 0);
  Call2(Func_02002af4, 7, 0);
  Call2(Func_02002b04, 0x1c, 0);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_ArmCallback, 0x1c, 0xd000, 0);
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
    Call1(FieldEvent_Wait, 0xf);
    Call2(Func_02002af4, 0x2019, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    Call1(FieldEvent_Wait, 0x23);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    Call2(Func_02002af4, 0x2019, 0);
  }
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call2(Func_02002b04, 4, 0x8000);
  Call1(FieldEvent_Wait, 0xa);
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
  Call3(Actor_ArmCallback, 0x1c, 0xd000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call2(Func_02002b04, 4, 0xc000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002b04, 0x19, 0x3000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x109, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call1(FieldEvent_Wait, 5);
  FieldMessage_ShowAtActor(0x2019, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call2(Func_02002b04, 0x1c, 0);
  t67 = Value2(Event_AskYesNoWithActorAnimation, 4, 0);
  if (t67 == 0) {
    Call1(FieldEvent_Wait, 0x14);
    Call2(Motion_SetModeAndWaitAnimation, 6, 3);
    Call1(FieldEvent_Wait, 0xa);
    Call2(Func_02002b04, 6, 0xc000);
    Call1(FieldEvent_Wait, 0xa);
    Call2(Func_02002af4, 6, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    Call1(FieldEvent_Wait, 0x28);
    Call2(Motion_SetModeAndWaitAnimation, 6, 3);
    Call1(FieldEvent_Wait, 0xa);
    Call2(Func_02002b04, 6, 0xc000);
    Call1(FieldEvent_Wait, 0xa);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    Call2(Func_02002af4, 6, 0);
  }
  Call2(Func_02002b04, 0x19, 0x5000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x105, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call2(Func_02002b04, 0x1c, 0xd000);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 4);
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
  Call2(Actor_StartRepeatedMotion, 0x1c, 3);
  Call2(Func_02002b24, 0x1c, 0x102);
  Call1(FieldEvent_Wait, 0x28);
  Call3(Actor_ArmCallback, 0x19, 0x3000, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call2(Func_02002b04, 0x1c, 0);
  Call3(Actor_SetPositionAndReset, 0x1a, 0x158, 0x268);
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
  Call2(Motion_SetModeAndWaitAnimation, 0x1a, 3);
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
  Call2(Actor_StartRepeatedMotion, 0x1c, 3);
  Call2(Func_02002b24, 0x1c, 0x102);
  Call1(FieldEvent_Wait, 0x28);
  Call3(Actor_ArmCallback, 0x19, 0x5000, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0x8000, 0);
  Call3(Actor_ArmCallback, 7, 0x8000, 0);
  Call2(Func_02002b04, 0x1c, 0x8000);
  Call3(Actor_SetPositionAndReset, 0x1b, 0xe8, 0x268);
  Call1(FieldEvent_Wait, 0xa);
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
  Call2(Func_02002b04, 0x1a, 0xb000);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_CommitPositionAndActivate, 0x1b, 0x10, 0);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_Jump, 0x1b, 4, 0x19);
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Func_02002b04, 0x1b, 0xd000);
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0xf);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call2(Func_02002b04, 6, 0);
  Call1(FieldEvent_Wait, 0xa);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x103, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x103, 0x32)
  Call2(Func_02002b04, 0x19, 0x3000);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call2(Func_02002b04, 0x1c, 0xd000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002b04, 0x19, 0x5000);
  Call1(FieldEvent_Wait, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x100, 0x1e)
  Call2(Func_02002af4, 7, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x105, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x106, 0x2a)
  Call3(Actor_CommitPositionAndActivate, 0x1c, 0, -16);
  Call2(Func_02002b04, 0x1c, 0xd000);
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Func_02002b04, 0x19, 0xd000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0x1e)
  Call2(Func_02002af4, 0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x109, 0x1e)
  Call2(Func_02002b04, 0x19, 0x3000);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Func_02002b04, 0x19, 0x5000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 7, 2);
  Call2(Func_02002af4, 7, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_SetSpeedParameters, 0x1c, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 0x1c, 8, -8);
  Call1(FieldEvent_Wait, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x108, 0x1e)
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_SetSpeedParameters, 0x19, 0x10000, 0x8000);
  t216 = Value1(Actor_GetById, 0x19);
  FIELD_AT((t216 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t216 + 0x5a), u8, 0));
  Call3(Actor_CommitPositionAndActivate, 0x19, -7, 7);
  Call1(FieldEvent_Wait, 1);
  t219 = Value1(Actor_GetById, 0x19);
  FIELD_AT((t219 + 0x5a), u8, 0) = (FIELD_AT((t219 + 0x5a), u8, 0) | 1);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002b04, 0x19, 0x5000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x28)
  Call2(Func_02002af4, 0x2019, 0);
  Call1(FieldEvent_Wait, 0xa);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x105, 0x32)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x105, 0x1e)
  Call2(Func_02002af4, 0x2005, 0);
  Call2(Actor_StartRepeatedMotion, 0x19, 3);
  Call2(Func_02002b24, 0x19, 0x102);
  Call1(FieldEvent_Wait, 0x28);
  Call2(Motion_SetVarCbAndRefresh, 0x1a, 2);
  Call2(Func_02002b04, 0x1a, 0xb000);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_SetSpeedParameters, 0x19, 0x9999, 0x4ccc);
  Call2(Func_02002b04, 0x19, 0);
  Call3(Actor_OffsetPositionAndReset, 0x19, 8, 0);
  Call2(Func_02002b04, 0x19, 0x3000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x101, 0x1e)
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Func_02002b04, 0x19, 0x5000);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1a, 0x105, 0x1e)
  Call2(Func_02002af4, 0x1a, 0);
  Call2(Func_02002b04, 0x19, 0x3000);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x102, 0x1e)
  Call2(Func_02002b04, 0x1b, 0);
  Call1(FieldEvent_Wait, 5);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call2(Func_02002b04, 7, 0);
  Call1(FieldEvent_Wait, 0x1e);
  Call3(Actor_SetSpeedParameters, 0x1c, 0x9999, 0x4ccc);
  Call2(Func_02002b04, 0x1c, 0x5000);
  Call1(FieldEvent_Wait, 2);
  t263 = Value1(Actor_GetById, 0x1c);
  FIELD_AT((t263 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t263 + 0x5a), u8, 0));
  Call3(Actor_CommitPositionAndActivate, 0x1c, -8, 8);
  Call1(FieldEvent_Wait, 1);
  t266 = Value1(Actor_GetById, 0x1c);
  FIELD_AT((t266 + 0x5a), u8, 0) = (1 | FIELD_AT((t266 + 0x5a), u8, 0));
  Call2(Func_02002b04, 0x1c, 0x5000);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 4);
  Call2(Func_02002af4, 0x201c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1a, 0x103, 0x2a)
  Call3(Actor_Jump, 0x1a, 6, 0xf);
  Call3(Actor_Jump, 0x1a, 6, 0x17);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call2(Func_02002b04, 0x1c, 0);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002b04, 0x1a, 0x8000);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_Jump, 0x1b, 6, 0xf);
  Call3(Actor_Jump, 0x1b, 6, 0x17);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_SetSpeedParameters, 0x1a, 0x1cccc, 0xe666);
  Call3(Actor_CommitPositionAndActivate, 0x1a, -12, 0);
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
  Call3(Actor_SetSpeedParameters, 0x1b, 0x1cccc, 0xe666);
  Call3(Actor_CommitPositionAndActivate, 0x1b, 0xc, 0);
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
  Call3(Actor_ArmCallback, 0x1a, 0xb000, 0);
  Call3(Actor_ArmCallback, 0x1b, 0xd000, 0);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call2(Func_02002b04, 0x1c, 0xd000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1a, 0x107, 0x1e)
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 5, 0xe000, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call3(Actor_ArmCallback, 0x19, 0x3000, 0);
  Call2(Func_02002b04, 0x1c, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x103, 0x2a)
  Call3(Actor_Jump, 0x1b, 6, 0xf);
  Call3(Actor_Jump, 0x1b, 6, 0x17);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 7, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0xa000, 0);
  Call3(Actor_ArmCallback, 6, 0xa000, 0);
  Call3(Actor_ArmCallback, 0x19, 0x5000, 0);
  Call2(Func_02002b04, 0x1c, 0x5000);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 0x1a, 2);
  Call2(Func_02002b04, 0x1a, 0xb000);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 5, 0xe000, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call3(Actor_ArmCallback, 0x19, 0x3000, 0);
  Call2(Func_02002b04, 0x1c, 0);
  Call1(FieldEvent_Wait, 0x14);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x109, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 0x1b, 2);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 7, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0xa000, 0);
  Call3(Actor_ArmCallback, 6, 0xa000, 0);
  Call3(Actor_ArmCallback, 0x19, 0x5000, 0);
  Call2(Func_02002b04, 0x1c, 0x5000);
  Call1(FieldEvent_Wait, 0xf);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0x19);
  Call2(Motion_SetVarCbAndRefresh, 0x1a, 2);
  Call2(Func_02002af4, 0x1a, 0);
  Call2(Func_02002b04, 0x19, 0x3000);
  Call2(Func_02002af4, 0x2019, 0);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Motion_SetVarCbAndRefresh, 0x1b, 2);
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Func_02002b04, 0x19, 0x5000);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call2(Func_02002b04, 0x1c, 0xd000);
  Call1(FieldEvent_Wait, 0xa);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x101, 0x1e)
  Call2(Func_02002af4, 0x2005, 0);
  Call2(Func_02002b04, 0x19, 0x3000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Func_02002b04, 0x19, 0x5000);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 7, 2);
  Call2(Func_02002af4, 7, 0);
  Call2(Func_02002b04, 0x19, 0xd000);
  Call1(FieldEvent_Wait, 0xf);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x1a, 3);
  Call2(Func_02002af4, 0x1a, 0);
  Call2(Func_02002b04, 0x19, 0x3000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Motion_SetVarCbAndRefresh, 0x1b, 2);
  Call2(Func_02002af4, 0x1b, 0);
  Call2(Func_02002b04, 0x1b, 0);
  Call2(Func_02002af4, 0x1b, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 7, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0x8000, 0);
  Call2(Func_02002b04, 0x1c, 0x8000);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Actor_WatchAndSetScript, 4, 0x1b);
  Call2(Actor_WatchAndSetScript, 7, 0x1b);
  Call2(Actor_WatchAndSetScript, 5, 0x1b);
  Call2(Actor_WatchAndSetScript, 6, 0x1b);
  Call2(Actor_WatchAndSetScript, 0x1c, 0x1b);
  Call3(Actor_SetSpeedParameters, 0x1b, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 0x1b, 0x14, 0);
  Call3(Actor_CommitPositionAndActivate, 0x1b, 2, 0x20);
  Call3(Actor_CommitPositionAndActivate, 0x1b, 0x20, 0x20);
  Call3(Actor_CommitPositionAndActivate, 0x1b, 0, 0x20);
  Call2(Func_02002b04, 0x1a, 0x8000);
  Call2(Motion_SetModeAndWaitAnimation, 0x1a, 3);
  Call2(Func_02002af4, 0x1a, 0);
  Call3(Actor_ArmCallback, 4, 0, 0);
  Call3(Actor_ArmCallback, 7, 0, 0);
  Call3(Actor_ArmCallback, 5, 0, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call2(Func_02002b04, 0x1c, 0);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Actor_WatchAndSetScript, 4, 0x1a);
  Call2(Actor_WatchAndSetScript, 7, 0x1a);
  Call2(Actor_WatchAndSetScript, 5, 0x1a);
  Call2(Actor_WatchAndSetScript, 6, 0x1a);
  Call2(Actor_WatchAndSetScript, 0x1c, 0x1a);
  Call3(Actor_SetSpeedParameters, 0x1a, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 0x1a, 0, 0x20);
  Call3(Actor_CommitPositionAndActivate, 0x1a, -20, 0x18);
  Call3(Actor_CommitPositionAndActivate, 0x1a, 0, 0x30);
  Call3(Actor_ArmCallback, 4, 0xc000, 0);
  Call3(Actor_ArmCallback, 7, 0xc000, 0);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xc000, 0);
  Call2(Func_02002b04, 0x1c, 0xd000);
  Call1(FieldEvent_Wait, 0x14);
  Call3(Func_02002a9c, 0x1a, 0, 0);
  Call3(Func_02002a9c, 0x1b, 0, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Actor_WatchAndSetScript, 4, 0x19);
  Call2(Actor_WatchAndSetScript, 7, 0x19);
  Call2(Actor_WatchAndSetScript, 5, 0x19);
  Call2(Actor_WatchAndSetScript, 6, 0x19);
  Call2(Actor_WatchAndSetScript, 0x1c, 0x19);
  Call3(Actor_SetSpeedParameters, 0x19, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 0x19, 0x16, 0x18);
  Call2(Camera_FollowActor, 0x19, 1);
  Call3(Actor_CommitPositionAndActivate, 0x19, 0, 0x20);
  Call3(Actor_CommitPositionAndActivate, 0x19, -16, 0x10);
  Call3(Actor_CommitPositionAndActivate, 0x19, 0, 8);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x100, 0x28)
  Call2(Func_02002b04, 0x19, 0xb000);
  Call4(Camera_MoveWithinMapBounds, 0x1300000, -1, 0x2880000, 1);
  Func_02002b3c();
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 0x1c, 2);
  Call1(FieldEvent_Wait, 0xa);
  Call3(Actor_SetSpeedParameters, 0x1c, 0x10000, 0x8000);
  Call3(Actor_CommitPositionAndActivate, 0x1c, 0, 0x20);
  Call2(Func_02002b04, 0x1c, 0x3000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 3);
  Call2(Func_02002af4, 0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x1e)
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x107, 0x1e)
  Call2(Func_02002af4, 0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Actor_StartRepeatedMotion, 0x1c, 3);
  Call2(Func_02002b24, 0x1c, 0x102);
  Call1(FieldEvent_Wait, 0x28);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0, 0);
  Call2(Func_02002b04, 7, 0);
  Call1(FieldEvent_Wait, 0x28);
  Call3(Actor_ArmCallback, 4, 0x4000, 0);
  Call3(Actor_ArmCallback, 5, 0x4000, 0);
  Call3(Actor_ArmCallback, 6, 0x4000, 0);
  Call2(Func_02002b04, 7, 0x4000);
  Call1(FieldEvent_Wait, 0x19);
  Call3(Actor_SetSpeedParameters, 0x1c, 0x6666, 0x3333);
  t494 = Value1(Actor_GetById, 0x1c);
  FIELD_AT((t494 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t494 + 0x5a), u8, 0));
  Call3(Actor_CommitPositionAndActivate, 0x1c, 0, -24);
  Call1(FieldEvent_Wait, 1);
  t497 = Value1(Actor_GetById, 0x1c);
  FIELD_AT((t497 + 0x5a), u8, 0) = (1 | FIELD_AT((t497 + 0x5a), u8, 0));
  Call1(FieldEvent_Wait, 0xa);
  Call2(Actor_SetModeById, 0x1c, 8);
  Call1(FieldEvent_Wait, 0x1e);
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Func_02002b04, 6, 0x4000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0x19);
  Call2(Motion_SetVarCbAndRefresh, 5, 2);
  Call2(Func_02002af4, 0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x105, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call2(Func_02002b04, 7, 0);
  Call1(FieldEvent_Wait, 0x28);
  Call3(Actor_ArmCallback, 4, 0x4000, 0);
  Call2(Func_02002b04, 7, 0x4000);
  Call1(FieldEvent_Wait, 0xa);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x101, 0x1e)
  Call2(Func_02002af4, 7, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 3);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x19, 4);
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Motion_SetVarCbAndRefresh, 6, 2);
  Call2(Func_02002af4, 6, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Call2(Func_02002af4, 0x2019, 0);
  Call2(Func_02002b04, 0x19, 0x5000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2019, 0);
  Call1(Func_02002bc4, 0x4e);
  Call3(Actor_CommitPositionAndActivate, 0x19, 0, 0x50);
  Call3(Func_02002a9c, 0x19, 0, 0);
  Call1(FieldEvent_Wait, 0x1e);
  Call4(Camera_MoveWithinMapBounds, 0x1300000, -1, 0x2700000, 1);
  Func_02002b3c();
  Call1(FieldEvent_Wait, 0xa);
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
  Call1(FieldEvent_Wait, 0xa);
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
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x100, 0x1e)
  Call2(Func_02002af4, 6, 0);
  Call2(Actor_SetModeById, 0x1c, 1);
  Call1(FieldEvent_Wait, 0xa);
  Call3(Actor_SetSpeedParameters, 0x1c, 0xcccc, 0x6666);
  Call3(Actor_CommitPositionAndActivate, 0x1c, 0, 0x10);
  Call2(Func_02002b04, 0x1c, 0);
  Call1(FieldEvent_Wait, 5);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 4);
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_ArmCallback, 4, 0x8000, 0);
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call3(Actor_ArmCallback, 6, 0x8000, 0);
  Call2(Func_02002b04, 7, 0x8000);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 3);
  Call2(Func_02002af4, 0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x100, 0x1e)
  Call2(Func_02002af4, 0x2007, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 4);
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Actor_StartRepeatedMotion, 7, 3);
  Call2(Func_02002b24, 7, 0x102);
  Call1(FieldEvent_Wait, 0x2d);
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
  Call1(FieldEvent_Wait, 0x19);
  Call2(Func_02000c90, 5, 6);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x28)
  Call3(Actor_ArmCallback, 5, 0x8000, 0);
  Call2(Func_02002b04, 6, 0x8000);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Actor_SetModeById, 5, 4);
  Call2(Motion_SetModeAndWaitAnimation, 6, 4);
  Call11(Func_02002b0c, 5, 5, 5, 0x12, 8, 6, 6, 0xe, 8, 9, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 3);
  Call2(Func_02002af4, 0x1c, 0);
  Call2(Actor_StartRepeatedMotion, 4, 3);
  Call2(Func_02002b24, 4, 0x102);
  Call1(FieldEvent_Wait, 0x28);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call2(Func_02002b04, 6, 0xe000);
  Call1(FieldEvent_Wait, 0xa);
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
    Call1(FieldEvent_Wait, 0xf);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * CHAMPA_EMOTE. */
    CHAMPA_EMOTE(0x1c, 0x102, 0x1e)
    Call2(Func_02002af4, 0x1c, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    Call2(Func_02000e04, 5, 6);
    Call1(FieldEvent_Wait, 0x23);
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
  Call1(FieldEvent_Wait, 0xa);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x2d)
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 4);
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_Jump, 5, 6, 0xf);
  Call3(Actor_Jump, 5, 6, 0x17);
  Call2(Func_02002af4, 0x2005, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 3);
  Call2(Func_02002af4, 0x1c, 0);
  Call3(Actor_Jump, 6, 6, 0xf);
  Call3(Actor_Jump, 6, 6, 0x17);
  Call2(Func_02002af4, 0x2006, 0);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 3);
  Call2(Func_02002af4, 0x1c, 0);
  Call1(FieldEvent_Wait, 0xa);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x28)
  Call3(Actor_CommitPositionAndActivate, 0x1c, 0, -8);
  Call2(Func_02002b04, 0x1c, 0);
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
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2007, 0);
  Call1(FieldEvent_Wait, 5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x2d)
  Call2(Motion_SetModeAndWaitAnimation, 7, 3);
  Call1(FieldEvent_Wait, 0xa);
  Call2(Func_02002af4, 0x2007, 0);
  Call2(Motion_SetVarCbAndRefresh, 0x1c, 2);
  Call1(FieldEvent_Wait, 0x14);
  Call3(Actor_ArmCallback, 5, 0xc000, 0);
  Call3(Actor_ArmCallback, 6, 0xe000, 0);
  Call2(Func_02002b04, 7, 0);
  Call1(FieldEvent_Wait, 0x14);
  Call2(Motion_SetModeAndWaitAnimation, 4, 3);
  Call1(FieldEvent_Wait, 0xf);
  Call2(Actor_SetModeById, 5, 3);
  Call2(Actor_SetModeById, 6, 3);
  Call2(Actor_SetModeById, 7, 3);
  Call2(Motion_SetModeAndWaitAnimation, 0x1c, 3);
  Call1(FieldEvent_Wait, 0x14);
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
  Call3(Actor_SetSpeedParameters, 0x1c, 0x13333, 0x9999);
  Call2(Actor_SetModeById, 0x1c, 2);
  t691 = Value1(Actor_GetById, FIELD_AT(gMainRoutineWork, s32, 532));
  if (t691 != 0) {
    Call3(Actor_ResetAndSetPosition, 0x1c, FIELD_AT(t691, s16, 10),
          FIELD_AT(t691, s16, 18));
  }
  Call1(Actor_WaitForMoveById, 0x1c);
  Call3(Func_02002a9c, 0x1c, 0, 0);
  Func_02002a4c();
}
