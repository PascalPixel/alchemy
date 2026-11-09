#include "types.h"
#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "call.h"
#include "event_inline.h"

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Champa_MessageOpen(s32 speaker, s32 flags) {
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
    Func_080d47b4((actor), emote_, frames_);                                   \
  }
s32 Func_02002a44();
s32 Func_02002b3c();
s32 Func_02000c90();
s32 Func_02000d4c();
s32 Func_02000e04();
s32 Func_02002a4c();
extern u8 MsgChampa25[];
extern u8 gMainRoutineWork[];

void Func_02000e30(void) {
  s32 t0;
  s32 t216;
  s32 t219;
  s32 t263;
  s32 t266;
  s32 t494;
  s32 t497;
  s32 t673;
  s32 t679;
  s32 t685;
  s32 t691;

  t0 = Event_GetFlag(0x970);
  if (t0 == 0) {
    return;
  }
  GameFlag_SetBit(0x971);
  Func_02002a44();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgChampa25);
  Actor_SetSpeed(25, 0xcccc, 0x6666);
  Actor_SetSpeed(26, 0x10000, 0x8000);
  Actor_SetSpeed(27, 0x10000, 0x8000);
  Actor_SetPosition(25, 0x1380000, 0x2080000);
  Actor_SetPosition(26, 0x1c80000, 0x2680000);
  Actor_SetHorizontalPositionWithTerrain(27, 0xa80000, 0x2680000);
  Sound_Play(0x4e);
  Event_ShowMessage(0x19, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x101, 0x14)
  Actor_WalkToAndWait(4, 312, 616);
  Camera_MoveWithinMapBounds(0x1380000, -1, 0x2580000, 1);
  Func_02002b3c();
  Event_Wait(5);
  Sound_Play(0x27);
  Actor_SetPositionAndReset(25, 304, 576);
  Event_Wait(5);
  FieldActor_LaunchFromLeader(7, -16, 0, 0xc000);
  FieldActor_LaunchFromLeader(5, 0, 0x10, 0xc000);
  FieldActor_LaunchFromLeader(6, -16, 0x10, 0xc000);
  FieldActor_LaunchFromLeader(0x1c, -32, 0, 0xd000);
  Actor_WaitForMoveById(28);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x100, 0x1e)
  Event_ShowMessage(6, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(20);
  Actor_FaceDirectionAndWait(7, 0);
  Event_ShowMessage(7, 0);
  Actor_ArmCallbackAndRefresh(28, 0);
  Event_Wait(10);
  Event_ShowMessage(0x1c, 0);
  Actor_FaceDirection(28, 0xd000, 0);
  Actor_FaceDirectionAndWait(7, 0xc000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x100, 0x1e)
  Event_ShowMessage(7, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x101, 0x1e)
  Champa_MessageOpen(0x2019, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(15);
    Event_ShowMessageAndWait(0x2019, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    Event_Wait(35);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    Event_ShowMessageAndWait(0x2019, 0);
  }
  Actor_FaceDirection(7, 0, 0);
  Actor_ArmCallbackAndRefresh(4, 0x8000);
  Event_Wait(10);
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
  Event_ShowMessage(0x2005, 0);
  Actor_FaceDirection(28, 0xd000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirectionAndWait(4, 0xc000);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(25, 0x3000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x109, 0x1e)
  Event_ShowMessage(0x2019, 0);
  Event_Wait(5);
  FieldMessage_ShowAtActor(0x2019, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirectionAndWait(28, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    Motion_SetModeAndWaitAnimation(6, 3);
    Event_Wait(10);
    Actor_ArmCallbackAndRefresh(6, 0xc000);
    Event_Wait(10);
    Event_ShowMessageAndWait(6, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    Event_Wait(40);
    Motion_SetModeAndWaitAnimation(6, 3);
    Event_Wait(10);
    Actor_ArmCallbackAndRefresh(6, 0xc000);
    Event_Wait(10);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    Event_ShowMessageAndWait(6, 0);
  }
  Actor_FaceDirectionAndWait(25, 0x5000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x105, 0x1e)
  Event_ShowMessage(0x2019, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirectionAndWait(28, 0xd000);
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(0x1c, 0);
  Event_ShowMessage(0x1a, 0);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldActor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, 0x102);
  FieldActor_StartRepeatedMotion(7, 3);
  Actor_SetAttachedEffect(7, 0x102);
  FieldActor_StartRepeatedMotion(6, 3);
  Actor_SetAttachedEffect(6, 0x102);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(25, 0x3000, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirectionAndWait(28, 0);
  Actor_WalkToAndWait(26, 344, 616);
  Event_Wait(5);
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
  Actor_RunRepeatedMotion(6, 2);
  Event_ShowMessage(6, 0);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirectionAndWait(5, 0xc000);
  Event_ShowMessage(0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Event_ShowMessage(0x2019, 0);
  Event_ShowMessage(0x1b, 0);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldActor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, 0x102);
  FieldActor_StartRepeatedMotion(7, 3);
  Actor_SetAttachedEffect(7, 0x102);
  FieldActor_StartRepeatedMotion(6, 3);
  Actor_SetAttachedEffect(6, 0x102);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(25, 0x5000, 0);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirectionAndWait(28, 0x8000);
  Actor_WalkToAndWait(27, 232, 616);
  Event_Wait(10);
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
  Event_ShowMessage(0x2019, 0);
  Actor_FaceDirectionAndWait(26, 0xb000);
  Event_ShowMessage(0x1a, 0);
  Actor_WalkByAndWait(27, 16, 0);
  Event_ShowMessage(0x1b, 0);
  FieldActor_Jump(27, 4, 25);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirectionAndWait(27, 0xd000);
  Event_ShowMessage(0x1b, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(15);
  Event_ShowMessage(0x2019, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_ArmCallbackAndRefresh(6, 0);
  Event_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x103, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x103, 0x32)
  Actor_FaceDirectionAndWait(25, 0x3000);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirectionAndWait(28, 0xd000);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(25, 0x5000);
  Event_Wait(5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x100, 0x1e)
  Event_ShowMessage(7, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x105, 0x1e)
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x106, 0x2a)
  Actor_WalkByAndWait(28, 0, -16);
  Actor_FaceDirectionAndWait(28, 0xd000);
  Event_ShowMessage(0x1c, 0);
  Actor_FaceDirectionAndWait(25, 0xd000);
  Event_Wait(10);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0x1e)
  Event_ShowMessage(0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x109, 0x1e)
  Actor_FaceDirectionAndWait(25, 0x3000);
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x1e)
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(25, 0x5000);
  Event_Wait(10);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  Actor_RunRepeatedMotion(7, 2);
  Event_ShowMessage(7, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessage(0x2019, 0);
  Actor_SetSpeed(28, 0x10000, 0x8000);
  Actor_WalkByAndWait(28, 8, -8);
  Event_Wait(5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x108, 0x1e)
  Event_ShowMessage(0x1c, 0);
  Actor_SetSpeed(25, 0x10000, 0x8000);
  t216 = ((s32)Actor_GetById(25));
  FIELD_AT((t216 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t216 + 0x5a), u8, 0));
  Actor_CommitPositionAndActivate(25, -7, 7);
  Event_Wait(1);
  t219 = ((s32)Actor_GetById(25));
  FIELD_AT((t219 + 0x5a), u8, 0) = (FIELD_AT((t219 + 0x5a), u8, 0) | 1);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(25, 0x5000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x28)
  Event_ShowMessage(0x2019, 0);
  Event_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x105, 0x32)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x105, 0x1e)
  Event_ShowMessage(0x2005, 0);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(0x19, 0x102);
  Event_Wait(40);
  Actor_RunRepeatedMotion(26, 2);
  Actor_FaceDirectionAndWait(26, 0xb000);
  Event_ShowMessage(0x1a, 0);
  Actor_SetSpeed(25, 0x9999, 0x4ccc);
  Actor_FaceDirectionAndWait(25, 0);
  Actor_WalkBy(25, 8, 0);
  Actor_FaceDirectionAndWait(25, 0x3000);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x101, 0x1e)
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirectionAndWait(25, 0x5000);
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1a, 0x105, 0x1e)
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirectionAndWait(25, 0x3000);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x102, 0x1e)
  Actor_ArmCallbackAndRefresh(27, 0);
  Event_Wait(5);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_ArmCallbackAndRefresh(7, 0);
  Event_Wait(30);
  Actor_SetSpeed(28, 0x9999, 0x4ccc);
  Actor_FaceDirectionAndWait(28, 0x5000);
  Event_Wait(2);
  t263 = ((s32)Actor_GetById(28));
  FIELD_AT((t263 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t263 + 0x5a), u8, 0));
  Actor_CommitPositionAndActivate(28, -8, 8);
  Event_Wait(1);
  t266 = ((s32)Actor_GetById(28));
  FIELD_AT((t266 + 0x5a), u8, 0) = (1 | FIELD_AT((t266 + 0x5a), u8, 0));
  Actor_FaceDirectionAndWait(28, 0x5000);
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(0x201c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1a, 0x103, 0x2a)
  FieldActor_Jump(26, 6, 15);
  FieldActor_Jump(26, 6, 23);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_ArmCallbackAndRefresh(28, 0);
  Event_Wait(10);
  Actor_FaceDirectionAndWait(26, 0x8000);
  Event_ShowMessage(0x1a, 0);
  FieldActor_Jump(27, 6, 15);
  FieldActor_Jump(27, 6, 23);
  Event_ShowMessage(0x1b, 0);
  Actor_SetSpeed(26, 0x1cccc, 0xe666);
  Actor_CommitPositionAndActivate(26, -12, 0);
  Event_Wait(5);
  Event_ShowMessage(0x1a, 0);
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
  Actor_SetSpeed(27, 0x1cccc, 0xe666);
  Actor_CommitPositionAndActivate(27, 12, 0);
  Event_Wait(5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x108, 0x1e)
  Event_ShowMessage(0x1b, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x100, 0x14)
  Actor_FaceDirectionAndWait(6, 0x8000);
  Event_ShowMessage(0x2006, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Event_ShowMessage(0x2019, 0);
  Actor_FaceDirection(26, 0xb000, 0);
  Actor_FaceDirection(27, 0xd000, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirectionAndWait(28, 0xd000);
  Event_Wait(10);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1a, 0x107, 0x1e)
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  Actor_FaceDirectionAndWait(28, 0);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1b, 0x103, 0x2a)
  FieldActor_Jump(27, 6, 15);
  FieldActor_Jump(27, 6, 23);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirection(5, 0xa000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  Actor_FaceDirection(25, 0x5000, 0);
  Actor_FaceDirectionAndWait(28, 0x5000);
  Event_Wait(20);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  Actor_RunRepeatedMotion(26, 2);
  Actor_FaceDirectionAndWait(26, 0xb000);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  Actor_ArmCallbackAndRefresh(28, 0);
  Event_Wait(20);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x109, 0x1e)
  Event_ShowMessage(0x2019, 0);
  Actor_RunRepeatedMotion(27, 2);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirection(5, 0xa000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  Actor_FaceDirection(25, 0x5000, 0);
  Actor_FaceDirectionAndWait(28, 0x5000);
  Event_Wait(15);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(25);
  Actor_RunRepeatedMotion(26, 2);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirectionAndWait(25, 0x3000);
  Event_ShowMessage(0x2019, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(27, 2);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirectionAndWait(25, 0x5000);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirectionAndWait(28, 0xd000);
  Event_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x101, 0x1e)
  Event_ShowMessage(0x2005, 0);
  Actor_FaceDirectionAndWait(25, 0x3000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x1e)
  Event_ShowMessage(6, 0);
  Actor_FaceDirectionAndWait(25, 0x5000);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  Actor_RunRepeatedMotion(7, 2);
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(25, 0xd000);
  Event_Wait(15);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Event_ShowMessage(0x2019, 0);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirectionAndWait(25, 0x3000);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(20);
  Actor_RunRepeatedMotion(27, 2);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirectionAndWait(27, 0);
  Event_ShowMessage(0x1b, 0);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(7, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirectionAndWait(28, 0x8000);
  Event_Wait(20);
  FieldActor_WatchAndSetScript(4, 27);
  FieldActor_WatchAndSetScript(7, 27);
  FieldActor_WatchAndSetScript(5, 27);
  FieldActor_WatchAndSetScript(6, 27);
  FieldActor_WatchAndSetScript(28, 27);
  Actor_SetSpeed(27, 0x10000, 0x8000);
  Actor_WalkByAndWait(27, 20, 0);
  Actor_WalkByAndWait(27, 2, 32);
  Actor_WalkByAndWait(27, 32, 32);
  Actor_WalkByAndWait(27, 0, 32);
  Actor_FaceDirectionAndWait(26, 0x8000);
  Actor_SetAnimationAndWait(26, 3);
  Event_ShowMessage(0x1a, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(7, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_ArmCallbackAndRefresh(28, 0);
  Event_Wait(20);
  FieldActor_WatchAndSetScript(4, 26);
  FieldActor_WatchAndSetScript(7, 26);
  FieldActor_WatchAndSetScript(5, 26);
  FieldActor_WatchAndSetScript(6, 26);
  FieldActor_WatchAndSetScript(28, 26);
  Actor_SetSpeed(26, 0x10000, 0x8000);
  Actor_WalkByAndWait(26, 0, 32);
  Actor_WalkByAndWait(26, -20, 24);
  Actor_WalkByAndWait(26, 0, 48);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_ArmCallbackAndRefresh(28, 0xd000);
  Event_Wait(20);
  Actor_SetPosition(26, 0, 0);
  Actor_SetPosition(27, 0, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessage(0x2019, 0);
  FieldActor_WatchAndSetScript(4, 25);
  FieldActor_WatchAndSetScript(7, 25);
  FieldActor_WatchAndSetScript(5, 25);
  FieldActor_WatchAndSetScript(6, 25);
  FieldActor_WatchAndSetScript(28, 25);
  Actor_SetSpeed(25, 0x10000, 0x8000);
  Actor_WalkByAndWait(25, 22, 24);
  FieldCamera_FollowActor(25, 1);
  Actor_WalkByAndWait(25, 0, 32);
  Actor_WalkByAndWait(25, -16, 16);
  Actor_WalkByAndWait(25, 0, 8);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x100, 0x28)
  Actor_FaceDirectionAndWait(25, 0xb000);
  FieldCamera_MoveTo(0x1300000, -1, 0x2880000, 1);
  Func_02002b3c();
  Event_Wait(10);
  Event_ShowMessage(0x2019, 0);
  Motion_SetVarCbAndRefresh(28, 2);
  Event_Wait(10);
  Actor_SetSpeed(28, 0x10000, 0x8000);
  Actor_WalkByAndWait(28, 0, 32);
  Actor_ArmCallbackAndRefresh(28, 0x3000);
  Event_Wait(10);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessage(0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x1e)
  Event_ShowMessage(0x1c, 0);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x107, 0x1e)
  Event_ShowMessage(0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Event_ShowMessage(0x2019, 0);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0, 0);
  Actor_ArmCallbackAndRefresh(7, 0);
  Event_Wait(40);
  Actor_FaceDirection(4, 0x4000, 0);
  Actor_FaceDirection(5, 0x4000, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  Actor_FaceDirectionAndWait(7, 0x4000);
  Event_Wait(25);
  Actor_SetSpeed(28, 0x6666, 0x3333);
  t494 = ((s32)Actor_GetById(28));
  FIELD_AT((t494 + 0x5a), u8, 0) = (0xfe & FIELD_AT((t494 + 0x5a), u8, 0));
  Actor_CommitPositionAndActivate(28, 0, -24);
  Event_Wait(1);
  t497 = ((s32)Actor_GetById(28));
  FIELD_AT((t497 + 0x5a), u8, 0) = (1 | FIELD_AT((t497 + 0x5a), u8, 0));
  Event_Wait(10);
  Actor_SetModeById(28, 8);
  Event_Wait(30);
  Event_ShowMessage(0x1c, 0);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  Actor_FaceDirectionAndWait(6, 0x4000);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x1e)
  Event_ShowMessage(6, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(25);
  Actor_RunRepeatedMotion(5, 2);
  Event_ShowMessage(0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x105, 0x1e)
  Event_ShowMessage(0x2019, 0);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_ArmCallbackAndRefresh(7, 0);
  Event_Wait(40);
  Actor_FaceDirection(4, 0x4000, 0);
  Actor_ArmCallbackAndRefresh(7, 0x4000);
  Event_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(4, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x101, 0x1e)
  Event_ShowMessage(7, 0);
  Motion_SetModeAndWaitAnimation(25, 3);
  Event_Wait(10);
  Event_ShowMessage(0x2019, 0);
  Actor_SetAnimationAndWait(25, 4);
  Event_ShowMessage(0x2019, 0);
  Actor_RunRepeatedMotion(6, 2);
  Event_ShowMessage(6, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x19, 0x108, 0x1e)
  Event_ShowMessage(0x2019, 0);
  Actor_ArmCallbackAndRefresh(25, 0x5000);
  Event_Wait(10);
  Event_ShowMessageAndWait(0x2019, 0);
  Sound_Play(0x4e);
  Actor_WalkByAndWait(25, 0, 80);
  Actor_SetHorizontalPositionWithTerrain(25, 0, 0);
  Event_Wait(30);
  Camera_MoveWithinMapBounds(0x1300000, -1, 0x2700000, 1);
  Func_02002b3c();
  Event_Wait(10);
  Sound_Play(3);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0x1e)
  Event_ShowMessage(0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x103, 0x1e)
  Event_ShowMessage(6, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x109, 0x1e)
  Event_ShowMessage(7, 0);
  Actor_FaceDirectionAndWait(6, 0xc000);
  Event_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x1e)
  Event_ShowMessage(6, 0);
  Actor_RunRepeatedMotion(7, 2);
  Event_ShowMessage(7, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x100, 0x1e)
  Actor_FaceDirection(4, 0x6000, 0);
  Actor_ArmCallbackAndRefresh(5, 0xa000);
  Event_Wait(10);
  Event_ShowMessage(0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x100, 0x1e)
  Event_ShowMessage(6, 0);
  Actor_SetModeById(28, 1);
  Event_Wait(10);
  Actor_SetSpeed(28, 0xcccc, 0x6666);
  Actor_WalkByAndWait(28, 0, 16);
  Actor_ArmCallbackAndRefresh(28, 0);
  Event_Wait(5);
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(0x1c, 0);
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirectionAndWait(7, 0x8000);
  Event_Wait(20);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessage(0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x100, 0x1e)
  Event_ShowMessage(0x2007, 0);
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(0x1c, 0);
  FieldActor_StartRepeatedMotion(7, 3);
  Actor_SetAttachedEffect(7, 0x102);
  Event_Wait(45);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x101, 0x1e)
  Event_ShowMessage(0x2005, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x1e)
  Event_ShowMessage(0x2006, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_ArmCallbackAndRefresh(6, 1);
  Event_Wait(25);
  Call2(Func_02000c90, 5, 6);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x101, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x101, 0x28)
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_ArmCallbackAndRefresh(6, 0x8000);
  Event_Wait(10);
  Actor_SetAnimation(5, 4);
  Actor_SetAnimationAndWait(6, 4);
  FieldEvent_ShowTwoMessagesAndWait(5, 5, 5, 0x12, 8, 6, 6, 0xe, 8, 9, 0);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessage(0x1c, 0);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_ArmCallbackAndRefresh(6, 0xe000);
  Event_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x1e)
  Call11(Func_02000d4c, 5, 8, 5, 0x12, 8, 6, 8, 0xe, 8, 9, 0);
  Actor_FaceDirectionAndWait(4, 0x6000);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Func_02000e04(5, 6);
    Event_Wait(15);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * CHAMPA_EMOTE. */
    CHAMPA_EMOTE(0x1c, 0x102, 0x1e)
    Event_ShowMessageAndWait(0x1c, 0);
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
  } else {
    Func_02000e04(5, 6);
    Event_Wait(35);
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * CHAMPA_EMOTE. */
    CHAMPA_EMOTE(0x1c, 0x105, 0x1e)
    FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) =
        (FIELD_AT((FIELD_AT(0x3000000, s32, 108) + 0x1c4), u16, 0) + 1);
    Event_ShowMessageAndWait(0x1c, 0);
  }
  Actor_FaceDirection(4, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_ArmCallbackAndRefresh(6, 0x8000);
  Event_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(5, 0x107, 0)
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(6, 0x107, 0x2d)
  Actor_SetAnimationAndWait(28, 4);
  Event_ShowMessage(0x1c, 0);
  FieldActor_Jump(5, 6, 15);
  FieldActor_Jump(5, 6, 23);
  Event_ShowMessage(0x2005, 0);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessage(0x1c, 0);
  FieldActor_Jump(6, 6, 15);
  FieldActor_Jump(6, 6, 23);
  Event_ShowMessage(0x2006, 0);
  Actor_SetAnimationAndWait(28, 3);
  Event_ShowMessageAndWait(0x1c, 0);
  Event_Wait(10);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x28)
  Actor_WalkByAndWait(28, 0, -8);
  Actor_ArmCallbackAndRefresh(28, 0);
  Event_Wait(5);
  Event_ShowMessage(0x1c, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(7, 0x108, 0x1e)
  Event_ShowMessage(0x2007, 0);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x102, 0x1e)
  Event_ShowMessage(0x1c, 0);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessage(0x2007, 0);
  Event_Wait(5);
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * CHAMPA_EMOTE. */
  CHAMPA_EMOTE(0x1c, 0x101, 0x2d)
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessage(0x2007, 0);
  Motion_SetVarCbAndRefresh(28, 2);
  Event_Wait(20);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_ArmCallbackAndRefresh(7, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(15);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(7, 3);
  Motion_SetModeAndWaitAnimation(28, 3);
  Event_Wait(20);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetModeById(5, 2);
  t673 = ((s32)Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532)));
  if (t673 != 0) {
    Actor_ResetAndSetPosition(5, FIELD_AT(t673, s16, 10),
                              FIELD_AT(t673, s16, 18));
  }
  Actor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetModeById(6, 2);
  t679 = ((s32)Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532)));
  if (t679 != 0) {
    Actor_ResetAndSetPosition(6, FIELD_AT(t679, s16, 10),
                              FIELD_AT(t679, s16, 18));
  }
  Actor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetModeById(7, 2);
  t685 = ((s32)Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532)));
  if (t685 != 0) {
    Actor_ResetAndSetPosition(7, FIELD_AT(t685, s16, 10),
                              FIELD_AT(t685, s16, 18));
  }
  Actor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Actor_SetSpeed(28, 0x13333, 0x9999);
  Actor_SetModeById(28, 2);
  t691 = ((s32)Actor_GetById(FIELD_AT(gMainRoutineWork, s32, 532)));
  if (t691 != 0) {
    Actor_ResetAndSetPosition(28, FIELD_AT(t691, s16, 10),
                              FIELD_AT(t691, s16, 18));
  }
  Actor_WaitForMoveById(28);
  Actor_SetPosition(28, 0, 0);
  Func_02002a4c();
}
