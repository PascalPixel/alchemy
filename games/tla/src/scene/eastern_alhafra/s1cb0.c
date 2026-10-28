#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"
#include "map.h"

extern s32 gData_020054f4[];
extern s32 gData_0200556c[];
extern u32 gData_0200d5d0[];
extern u32 gData_02005620[];
extern s32 gData_02005670[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test();
s32 Event_AskYesNoWithActorAnimation();
void Func_02004b6c(s32 actor, s32 a, s32 b);
void FieldMessage_ShowAtActor(s32 actor, s32 flags);
void wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02004a5c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void FieldEvent_Wait();
void Event_StartScreenTransition();
void Func_02001bf4();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02004a94();
void Func_02004a9c();
void Func_02004b04();
void Func_02004b3c();
void Func_02004b4c();
void Func_02004b5c();
void Func_02004b74();
void Func_02004b84();
void Func_02004b94();
void Func_02004ba4();
void Func_02004bbc();
void Func_02004bc4();
void Func_02004be4();
void Func_02004bec();
void Func_02004c54();
void Func_02004c9c();
void GameFlag_SetBit();
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Motion_SetVarCbAndRefresh();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_CommitPositionAndActivate();
void Actor_Stop();
void Actor_SetMovementTypeById();
void Actor_Jump();
void Actor_OffsetPositionAndReset();
void Actor_ResetAndSetPosition();
void Actor_ResetAndSetPositionInMode2();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
void Actor_StartRepeatedMotion();
void Actor_SetSpritePriority();
void Camera_FollowActor();
void Actor_WatchAndSetScript();
void Actor_RefreshSelectorById();
void Actor_SetModeById();
void Actor_SetPositionAndResetMotion();

static __inline__ void Func_02004b6c_(s32 actor, s32 a, s32 b) {
  /* FAKEMATCH: pinned, the second and third arguments load before the actor, as
   * the native does at every call here (one reordered row per call without). */
  register s32 second asm("r1") = a;
  /* FAKEMATCH: the third argument, pinned with the second. */
  register s32 third asm("r2") = b;

  Func_02004b6c(actor, second, third);
}

static __inline__ void FieldMessage_ShowAtActor_(s32 actor, s32 flags) {
  /* FAKEMATCH: pinned, the flags load before the actor, as the native does (one
   * reordered row without). */
  register s32 f asm("r1") = flags;

  FieldMessage_ShowAtActor(actor, f);
}

/// The scene that opens with MsgHeyThatBlock: either answer to the prompt
/// steps the event to its next message.
void Func_02001cb0(void) {
  ((void (*)())Func_02004a94)();
  Call1((void (*)())Func_02004be4, 0);
  Call1((void (*)())Func_02004b3c, (s32)MsgHeyThatBlock);
  Call3((void (*)())Actor_SetSpeedParameters, 28, 0x2cccc, 0x16666);
  Call3((void (*)())Actor_SetSpeedParameters, 29, 0x2cccc, 0x16666);
  Call3((void (*)())Actor_SetSpeedParameters, 23, 0x1cccc, 0xe666);
  Call3((void (*)())Actor_SetSpeedParameters, 24, 0x1cccc, 0xe666);
  Call3((void (*)())Func_02004b04, 28, PIXELS(328), PIXELS(616));
  Call3((void (*)())Actor_ArmCallback, 28, FACING_NORTHEAST, 0);
  Call3((void (*)())Func_02004b04, 29, PIXELS(296), PIXELS(616));
  Call3((void (*)())Actor_ArmCallback, 29, FACING_NORTHEAST, 0);
  Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTH, 0);
  {
    /* FAKEMATCH: pinned, the mode loads before the z bound is shifted (one
     * reordered row without). */
    register s32 mode asm("r3") = 1;

    Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(328), -1, PIXELS(632),
          mode);
  }
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_StartRepeatedMotion, 28, 3);
  Call2((void (*)())Func_02004b74, 0x1c, 0x102);
  Call1((void (*)())FieldEvent_Wait, 50);
  Func_02004b6c_(0x1d, 0x102, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 28, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 28, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 29, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 29, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 29, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_OffsetPositionAndReset, 28, -160, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 29, -160, 0);
  Call3((void (*)())Func_02004b04, 28, 0, 0);
  Call3((void (*)())Func_02004b04, 29, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02004b04, 23, PIXELS(264), PIXELS(384));
  Call3((void (*)())Actor_ArmCallback, 23, FACING_EAST, 0);
  Call3((void (*)())Func_02004b04, 24, PIXELS(280), PIXELS(392));
  Call3((void (*)())Actor_ArmCallback, 24, FACING_EAST, 0);
  Call3((void (*)())Func_02004b04, 4, PIXELS(232), PIXELS(384));
  Call3((void (*)())Actor_ArmCallback, 4, FACING_EAST, 0);
  Call3((void (*)())Func_02004b4c, 0x18, 0, 0xa);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Call3((void (*)())Func_02004b4c, 0x18, 0, 0xa);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(264), -1, PIXELS(392),
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_CommitPositionAndActivate, 23, -16, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  FieldMessage_ShowAtActor_(0x17, 0);
  if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0) == 0) {
    Call1((void (*)())FieldEvent_Wait, 20);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 23, 3);
    Call1((void (*)())FieldEvent_Wait, 10);
    Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Call1((void (*)())FieldEvent_Wait, 40);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 23, 4);
    Call1((void (*)())FieldEvent_Wait, 10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  }
  Call3((void (*)())Actor_ArmCallback, 23, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 24, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Func_02004b6c_(0x18, 0x108, 0x2d);
  Call3((void (*)())Actor_Jump, 24, 6, 15);
  Call3((void (*)())Actor_Jump, 24, 6, 23);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 23, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 24, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 24, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_CommitPositionAndActivate, 23, 0, 8);
  Call2((void (*)())Actor_WatchAndSetScript, 4, 23);
  Call3((void (*)())Actor_OffsetPositionAndReset, 24, -96, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 23, -96, 0);
  Call3((void (*)())Actor_OffsetPositionAndReset, 23, 0, 80);
  Call3((void (*)())Actor_CommitPositionAndActivate, 24, -32, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 24, 0, 80);
  Call1((void (*)())Actor_Stop, 4);
  Call3((void (*)())Func_02004b04, 24, 0, 0);
  Call3((void (*)())Func_02004b04, 23, 0, 0);
  Call2((void (*)())Camera_FollowActor, 4, 1);
  ((void (*)())Func_02004b94)();
  ((void (*)())Func_02004a9c)();
}

/// Only once flag 0x8a7 is set: the scene that opens with MsgIStillJust, which
/// sets flag 0x905 and ends by sending the party to SceneId_Ov667, entrance 81.
void Func_02002048(void) {
  if (Value1((s32 (*)())GameFlag_Test, 0x8a7) == 0)
    return;
  Call1((void (*)())GameFlag_SetBit, 0x905);
  ((void (*)())Func_02004a94)();
  Call1((void (*)())Func_02004be4, 0);
  Call1((void (*)())Func_02004b3c, (s32)MsgIStillJust);
  Call2((void (*)())Func_02004b84, 0x40000, 0x8000);
  Call3((void (*)())Func_02004b04, 25, PIXELS(56), PIXELS(632));
  Call3((void (*)())Actor_ArmCallback, 25, FACING_EAST, 0);
  Call3((void (*)())Func_02004b04, 27, PIXELS(56), PIXELS(648));
  Call3((void (*)())Actor_ArmCallback, 27, FACING_EAST, 0);
  Call3((void (*)())Func_02004b04, 28, PIXELS(48), PIXELS(616));
  Call3((void (*)())Actor_ArmCallback, 28, FACING_EAST, 0);
  Call3((void (*)())Func_02004b04, 29, PIXELS(32), PIXELS(608));
  Call3((void (*)())Actor_ArmCallback, 29, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(144), -1, PIXELS(664),
        1);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_SetSpeedParameters, 28, 0x10000, 0x8000);
  Call3((void (*)())Actor_SetSpeedParameters, 29, 0x10000, 0x8000);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0x10000, 0x8000);
  Call3((void (*)())Actor_SetSpeedParameters, 27, 0x10000, 0x8000);
  Call3((void (*)())Actor_OffsetPositionAndReset, 25, 32, 0);
  Call3((void (*)())Actor_OffsetPositionAndReset, 27, 32, 0);
  Call3((void (*)())Actor_OffsetPositionAndReset, 28, 24, 24);
  Call3((void (*)())Actor_OffsetPositionAndReset, 29, 24, 24);
  Call1((void (*)())Actor_WaitForMoveById, 28);
  Call1((void (*)())Actor_WaitForMoveById, 29);
  Call3((void (*)())Actor_ArmCallback, 28, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 29, FACING_EAST, 0);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Func_02004b6c_(0x1c, 0x107, 0);
  Func_02004b6c_(0x1c, 0x107, 0x28);
  Call3((void (*)())Actor_SetSpeedParameters, 28, 0x30000, 0x18000);
  Call3((void (*)())Actor_SetSpeedParameters, 29, 0x30000, 0x18000);
  Call2((void (*)())Actor_SetMovementTypeById, 28, (s32)gData_020054f4);
  Call2((void (*)())Actor_SetMovementTypeById, 29, (s32)gData_0200556c);
  Call1((void (*)())Actor_RefreshSelectorById, 28);
  Call1((void (*)())Actor_RefreshSelectorById, 29);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 28, 2);
  Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
  Call3((void (*)())Actor_Jump, 29, 6, 15);
  Call3((void (*)())Actor_Jump, 29, 6, 23);
  Call3((void (*)())Func_02004b4c, 0x1d, 0, 0xa);
  Call3((void (*)())Actor_SetSpeedParameters, 28, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 29, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0x19999, 0xcccc);
  Call3((void (*)())Actor_SetSpeedParameters, 27, 0x19999, 0xcccc);
  Call2((void (*)())Actor_StartRepeatedMotion, 25, 3);
  Call2((void (*)())Func_02004b74, 0x19, 0x102);
  Call2((void (*)())Actor_StartRepeatedMotion, 27, 3);
  Call2((void (*)())Func_02004b74, 0x1b, 0x102);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Actor_SetMovementTypeById, 28, (s32)gData_0200d5d0);
  Call2((void (*)())Actor_SetMovementTypeById, 29, (s32)gData_02005620);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Camera_FollowActor, 25, 1);
  Call3((void (*)())Actor_OffsetPositionAndReset, 25, 64, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, 64, 0);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 25, 168, 616);
  Call3((void (*)())Actor_SetPositionAndReset, 27, 184, 616);
  Call1((void (*)())Actor_RefreshSelectorById, 25);
  Call2((void (*)())Actor_SetModeById, 25, 1);
  Call3((void (*)())Actor_ArmCallback, 25, FACING_NORTHEAST, 0);
  Call3((void (*)())Actor_ArmCallback, 27, FACING_NORTHEAST, 0);
  Call1((void (*)())Actor_RefreshSelectorById, 28);
  Call1((void (*)())Actor_RefreshSelectorById, 29);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_StartRepeatedMotion, 25, 3);
  Call2((void (*)())Func_02004b74, 0x19, 0x102);
  Call2((void (*)())Actor_StartRepeatedMotion, 27, 3);
  Call2((void (*)())Func_02004b74, 0x1b, 0x102);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call2((void (*)())Func_02004b84, 0x60000, 0xc000);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(384), -1, PIXELS(480),
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Func_02004b84, 0x60000, 0xc000);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(480), -1, PIXELS(344),
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Func_02004b84, 0x60000, 0xc000);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(488), -1, PIXELS(456),
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Func_02004b84, 0x60000, 0xc000);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(192), -1, PIXELS(616),
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Func_02004b6c_(0x19, 0x102, 0x1e);
  Call3((void (*)())Actor_ArmCallback, 25, FACING_NORTH, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 28, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, 29, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Actor_SetModeById, 28, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 29, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 27, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Func_02004b6c_(0x1b, 0x108, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x1b, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 25, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Actor_SetModeById, 28, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 29, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 25, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 27, 0xb000, 0);
  Call1((void (*)())FieldEvent_Wait, 40);
  Func_02004b6c_(0x19, 0x101, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Actor_StartRepeatedMotion, 28, 3);
  Call2((void (*)())Func_02004b74, 0x1c, 0x102);
  Call2((void (*)())Actor_StartRepeatedMotion, 29, 3);
  Call2((void (*)())Func_02004b74, 0x1d, 0x102);
  Call1((void (*)())FieldEvent_Wait, 50);
  Call3((void (*)())Actor_ArmCallback, 28, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 29, 0xb000, 0);
  Call1((void (*)())FieldEvent_Wait, 40);
  Func_02004b6c_(0x1c, 0x100, 0);
  Func_02004b6c_(0x1d, 0x100, 0x28);
  Call3((void (*)())Actor_ArmCallback, 25, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 27, 0xb000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(144), -1, PIXELS(544),
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Func_02004b6c_(4, 0x102, 0x1e);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0x18000, 0xc000);
  Call3((void (*)())Actor_SetSpeedParameters, 27, 0x18000, 0xc000);
  Call3((void (*)())Actor_OffsetPositionAndReset, 25, -32, -32);
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, -32, -32);
  Call3((void (*)())Actor_OffsetPositionAndReset, 25, 0, -24);
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, 0, -24);
  Call1((void (*)())Actor_WaitForMoveById, 25);
  Call3((void (*)())Actor_ArmCallback, 25, 0xd000, 0);
  Call3((void (*)())Actor_ArmCallback, 27, 0xb000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Func_02004b6c_(0x19, 0x100, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 136, 520);
  Call4((void (*)())Func_02004bec, 0x1a, 0x10, 0, 0x5000);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTH, 0);
  Call1((void (*)())Actor_WaitForMoveById, 26);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 26, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 26, 4);
  Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
  Func_02004b6c_(0x19, 0x109, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 26, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 27, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 26, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 27, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x1b, 0, 0xa);
  Func_02004b6c_(0x19, 0x100, 0x1e);
  Call3((void (*)())Actor_ArmCallback, 25, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 27, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, 25, 0x3000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_Jump, 25, 6, 15);
  Call3((void (*)())Actor_Jump, 25, 6, 23);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 28, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x1c, 0, 0xa);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 29, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x1d, 0, 0xa);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0x14);
  Call3((void (*)())Actor_OffsetPositionAndReset, 28, 0, 64);
  Call3((void (*)())Actor_OffsetPositionAndReset, 29, 0, 64);
  Call1((void (*)())FieldEvent_Wait, 60);
  Call3((void (*)())Func_02004b04, 28, 0, 0);
  Call3((void (*)())Func_02004b04, 29, 0, 0);
  Call3((void (*)())Actor_ArmCallback, 27, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 25, 0xd000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  FieldMessage_ShowAtActor_(0x19, 0);
  if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0) != 0) {
    Call1((void (*)())FieldEvent_Wait, 40);
    Call3((void (*)())Actor_ArmCallback, 26, FACING_WEST, 0);
    Call1((void (*)())FieldEvent_Wait, 20);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 26, 4);
    Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Call1((void (*)())FieldEvent_Wait, 30);
    Call3((void (*)())Actor_ArmCallback, 26, FACING_WEST, 0);
    Call1((void (*)())FieldEvent_Wait, 20);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 26, 3);
    Call1((void (*)())FieldEvent_Wait, 10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Call3((void (*)())Func_02004b4c, 0x201a, 0, 0xa);
  }
  Call3((void (*)())Actor_ArmCallback, 4, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 3);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTH, 0);
  Call3((void (*)())Actor_ArmCallback, 26, 0x3000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 25, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, 27, 0x3000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_SetSpeedParameters, 4, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 26, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 27, 0xcccc, 0x6666);
  Actor_GetById(4)->motionFlags = 0;
  Actor_GetById(26)->motionFlags = 0;
  Actor_GetById(25)->motionFlags = 0;
  Actor_GetById(27)->motionFlags = 0;
  Call3((void (*)())Actor_OffsetPositionAndReset, 4, 0, 120);
  Call3((void (*)())Actor_OffsetPositionAndReset, 26, 0, 120);
  Call3((void (*)())Actor_OffsetPositionAndReset, 27, 0, 80);
  Call3((void (*)())Actor_CommitPositionAndActivate, 25, 0, 80);
  {
    struct EventWork *event = Ram_HeapSlots->eventWork;

    event->delay = 60;
    event->screenEffect = 0x100;
  }
  ((void (*)())Event_FinishScreenTransition)();
  ((void (*)())Event_WaitTransitionFrames)();
  Call2((void (*)())Func_02004ba4, (s32)&SceneId_Ov667, 81);
  ((void (*)())Func_02004a9c)();
}

/// Only once flag 0x8a7 is set: the scene that opens with MsgBriggs2, which sets
/// flags 0x90f and 0x97f and ends by sending the party to SceneId_Ov668Room1,
/// entrance 99.
void Func_02002a08(void) {
  struct MapWork *map;
  s32 i;
  s32 step;

  if (Value1((s32 (*)())GameFlag_Test, 0x8a7) == 0)
    return;
  Call1((void (*)())GameFlag_SetBit, 0x90f);
  Call1((void (*)())GameFlag_SetBit, 0x97f);
  ((void (*)())Func_02004a94)();
  Call1((void (*)())Func_02004be4, 0);
  Call1((void (*)())Func_02004b3c, (s32)MsgBriggs2);
  Call3((void (*)())Func_02004b4c, 0x1d, 0, 0xa);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 136, 552);
  Call4((void (*)())Func_02004bec, 0x1a, -0x10, 0, 0xa000);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call1((void (*)())Actor_WaitForMoveById, 26);
  Call1((void (*)())FieldEvent_Wait, 20);
  Func_02004b6c_(0x1a, 0x100, 0x1e);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(176), -1, PIXELS(392),
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 25, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 27, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x1b, 0, 0xa);
  Call3((void (*)())Func_02004b04, 22, PIXELS(224), PIXELS(8));
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Ram_HeapSlots->mapWork->minX = 0;
  Ram_HeapSlots->mapWork->maxX = 0x4000000;
  Ram_HeapSlots->mapWork->minZ = 0;
  Ram_HeapSlots->mapWork->maxZ = 0x4000000;
  Call2((void (*)())Func_02004b84, 0x40000, 0x8000);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(208), -1, PIXELS(168),
        1);
  Call1((void (*)())FieldEvent_Wait, 50);
  Call1((void (*)())Func_02004c9c, 0x4e);
  ((void (*)())Event_FinishScreenTransition)();
  Call1((void (*)())Func_02004bc4, 0x10);
  ((void (*)())Event_WaitTransitionFrames)();
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(808), -1, PIXELS(664),
        0);
  {
    struct Actor *center =
        ((struct EventWork *)Ram_HeapSlots->eventWork)->viewCenter;

    Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)center, center->x,
          center->y, center->z);
  }
  Call1((void (*)())FieldEvent_Wait, 50);
  Call2((void (*)())Actor_SetSpritePriority, 22, 1);
  Call2((void (*)())Actor_SetSpritePriority, 23, 1);
  Call3((void (*)())Actor_SetSpeedParameters, 23, 0x13333, 0x9999);
  Call3((void (*)())Actor_SetSpeedParameters, 22, 0x13333, 0x9999);
  Call3((void (*)())Func_02004b04, 22, PIXELS(792), PIXELS(856));
  Call3((void (*)())Func_02004b04, 23, PIXELS(824), PIXELS(856));
  Call3((void (*)())Func_02004b04, 26, PIXELS(744), PIXELS(1016));
  Call3((void (*)())Func_02004b04, 25, PIXELS(744), PIXELS(1016));
  Call1((void (*)())Func_02004c9c, 0x23);
  Call2((void (*)())Func_02004bbc, 0x10000, 0);
  ((void (*)())Event_StartScreenTransition)();
  ((void (*)())Event_WaitTransitionFrames)();
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Func_02004b84, 0x20000, 0x4000);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(808), -1, PIXELS(848),
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_Jump, 22, 6, 15);
  Call3((void (*)())Actor_Jump, 22, 6, 23);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 23, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 22, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 23, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 25);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 22, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 23, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_ArmCallback, 22, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 23, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Call3((void (*)())Func_02004b4c, 0x1a, 0, 0xa);
  Func_02004b6c_(0x17, 0x100, 0);
  Func_02004b6c_(0x16, 0x100, 0x32);
  Call2((void (*)())Actor_StartRepeatedMotion, 22, 3);
  Call2((void (*)())Func_02004b74, 0x16, 0x102);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Actor_CommitPositionAndActivate, 22, 0, 16);
  Call1((void (*)())FieldEvent_Wait, 20);
  Func_02004b6c_(0x16, 0x102, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Call3((void (*)())Actor_CommitPositionAndActivate, 23, 0, 16);
  Call3((void (*)())Actor_ArmCallback, 23, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Func_02004b6c_(0x17, 0x109, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 23, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 50);
  Call3((void (*)())Actor_ArmCallback, 23, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 22, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Func_02004b6c_(0x16, 0x101, 0x2d);
  Call3((void (*)())Actor_CommitPositionAndActivate, 22, 0, 32);
  Call1((void (*)())FieldEvent_Wait, 30);
  Func_02004b6c_(0x16, 0x106, 0x2d);
  Call3((void (*)())Actor_CommitPositionAndActivate, 22, 0, -32);
  Call3((void (*)())Actor_ArmCallback, 22, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 22, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 23, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 50);
  Call3((void (*)())Actor_ArmCallback, 23, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Func_02004b6c_(0x17, 0x108, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Func_02004b6c_(0x16, 0x101, 0x32);
  Call3((void (*)())Actor_CommitPositionAndActivate, 22, 0, 32);
  Call1((void (*)())FieldEvent_Wait, 30);
  Func_02004b6c_(0x16, 0x106, 0x32);
  Call3((void (*)())Actor_CommitPositionAndActivate, 22, 0, -32);
  Call3((void (*)())Actor_ArmCallback, 22, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 22, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Func_02004b6c_(0x17, 0x109, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 22, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call1((void (*)())Func_02001bf4, 0x16);
  Call1((void (*)())Func_02004c9c, 0x67);
  Func_02004b6c_(0x16, 0x105, 0x32);
  Call3((void (*)())Actor_ArmCallback, 22, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 23, 4);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 22, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call1((void (*)())Func_02001bf4, 0x16);
  Func_02004b6c_(0x16, 0x106, 0x32);
  Call3((void (*)())Actor_ArmCallback, 22, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 22, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Func_02004b6c_(0x17, 0x109, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 22, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 22, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Func_02004b5c, 22, 0x5000);
  Call2((void (*)())Actor_SetModeById, 22, 7);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_SetModeById, 22, 6);
  Call1((void (*)())FieldEvent_Wait, 5);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Call3((void (*)())Actor_Jump, 23, 6, 15);
  Call3((void (*)())Actor_Jump, 23, 6, 23);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Func_02004b6c_(0x16, 0x102, 0x32);
  Call2((void (*)())Actor_SetModeById, 22, 7);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_SetModeById, 22, 1);
  Call3((void (*)())Actor_ArmCallback, 22, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 15);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Func_02004b6c_(0x17, 0x103, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x17, 0, 0xa);
  Func_02004b6c_(0x16, 0x106, 0x28);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Call3((void (*)())Actor_CommitPositionAndActivate, 22, 0, 8);
  Call3((void (*)())Actor_CommitPositionAndActivate, 22, 10, 16);
  Call3((void (*)())Actor_ArmCallback, 23, 0x5000, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 22, 6, 16);
  Call3((void (*)())Actor_ArmCallback, 22, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_Jump, 22, 6, 15);
  Call3((void (*)())Actor_Jump, 22, 6, 23);
  Call3((void (*)())Func_02004b4c, 0x4016, 0, 0xa);
  Call3((void (*)())Func_02004b4c, 0x1a, 0, 0xa);
  Func_02004b6c_(0x16, 0x108, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x4016, 0, 0x14);
  Call2((void (*)())Actor_SetModeById, 22, 8);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0xa);
  Call3((void (*)())Actor_Jump, 22, 7, 0);
  i = 15;
turn:
  // The angle, through s16: as the u16 field its halfword zero is shared with
  // the motion-flag stores below and loads from the pool (differ 19 more).
  ((s16 *)Actor_GetById(22))[3] -= 0x1000;
  i--;
  wait(1);
  if (i >= 0)
    goto turn;
  Call2((void (*)())Actor_SetModeById, 22, 5);
  Call1((void (*)())FieldEvent_Wait, 7);
  Call3((void (*)())Func_02004b4c, 0x16, 0, 0x14);
  Func_02004b6c_(0x17, 0x108, 0x32);
  Call3((void (*)())Actor_CommitPositionAndActivate, 23, 0, 36);
  Call1((void (*)())FieldEvent_Wait, 15);
  Call2((void (*)())Actor_SetModeById, 22, 1);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_Jump, 23, 6, 15);
  Call3((void (*)())Actor_Jump, 23, 6, 23);
  Call3((void (*)())Func_02004b4c, 0x4017, 0, 0xf);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 22, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 22, 3);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_SetModeById, 22, 9);
  Call1((void (*)())FieldEvent_Wait, 70);
  map = Ram_HeapSlots->mapWork;
  Actor_GetById(22)->motionFlags = 0;
  Actor_GetById(23)->motionFlags = 0;
  Func_02004a5c(42, 60, 17, 4, 42, 64);
  Func_02004a5c(42, 60, 17, 4, 42, 68);
  Func_02004a5c(106, 60, 17, 4, 106, 64);
  Func_02004a5c(106, 60, 17, 4, 106, 68);
  /* FAKEMATCH: the step in a local, used for the first layer and both actors,
   * gives the native's registers (differ 14 written as the literal each time).
   */
  step = 0x10000;
  i = 239;
scroll:
  *(s32 *)(map->layers[0].unk08 + 4) += step;
  *(s32 *)(map->layers[1].unk08 + 4) += 0x10000;
  Actor_GetById(22)->z -= step;
  i--;
  Actor_GetById(23)->z -= step;
  wait(1);
  if (i >= 0)
    goto scroll;
  Call1((void (*)())Func_02004c9c, 0x4e);
  Call1((void (*)())FieldEvent_Wait, 30);
  ((void (*)())Func_02004a9c)();
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call2((void (*)())Func_02004ba4, (s32)&SceneId_Ov668Room1, 99);
}

/// The scene that opens with MsgWhatYouLet; either answer to each of its two
/// prompts steps the event's message number, by 1 and then by 2.
void Func_02003274(void) {
  ((void (*)())Func_02004a94)();
  Call1((void (*)())Func_02004be4, 0);
  Call1((void (*)())Func_02004b3c, (s32)MsgWhatYouLet);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(152), -1, PIXELS(272),
        0);
  {
    struct Actor *center =
        ((struct EventWork *)Ram_HeapSlots->eventWork)->viewCenter;

    Call4((void (*)())Actor_SetPositionAndResetMotion, (s32)center, center->x,
          center->y, center->z);
  }
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Func_02004bbc, 0x10000, 0);
  ((void (*)())Event_StartScreenTransition)();
  ((void (*)())Event_WaitTransitionFrames)();
  Call1((void (*)())FieldEvent_Wait, 15);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call4((void (*)())Camera_MoveWithinMapBounds, PIXELS(152), -1, PIXELS(392),
        1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 20);
  Func_02004b6c_(0x19, 0x103, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x1019, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 25, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02004b4c, 0x1019, 0, 0xa);
  Call3((void (*)())Actor_Jump, 25, 6, 15);
  Call3((void (*)())Actor_Jump, 25, 6, 23);
  Call1((void (*)())FieldEvent_Wait, 10);
  Func_02004b6c_(0x19, 0x109, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x1019, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 25, 0xd000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Func_02004b6c_(0x19, 0x100, 0x1e);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0xb333, 0x5999);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 25, 0, 8);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(25)->actionFlags |= 1;
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 25, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0x26666, 0x13333);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 25, 0, -16);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(25)->actionFlags |= 1;
  Actor_GetById(25)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_OffsetPositionAndReset, 25, 0, 8);
  Call1((void (*)())Func_02004c9c, 0x85);
  Call3((void (*)())Actor_SetSpeedParameters, 29, 0x26666, 0x13333);
  Actor_GetById(29)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 29, 0, -4);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = FACING_WEST;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 29, 0, -4);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = 0xb000;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 29, 0, -4);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = 0xd000;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 29, 0, -4);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = 0;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 29, 0, -4);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = 0x3000;
  Call1((void (*)())FieldEvent_Wait, 20);
  Actor_GetById(25)->actionFlags |= 1;
  Call3((void (*)())Func_02004b4c, 0x1019, 0, 0xa);
  Call3((void (*)())Func_02004b04, 29, PIXELS(168), PIXELS(368));
  Call2((void (*)())Func_02004c54, 0x1d, 0);
  Actor_SetSingleChildFlags(Actor_GetById(29), 1);
  Call3((void (*)())Actor_ArmCallback, 29, 0x5000, 0);
  Call3((void (*)())Actor_Jump, 29, 6, 40);
  Func_02004b6c_(0x1d, 0x102, 0x28);
  Call3((void (*)())Actor_ArmCallback, 25, 0x3000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0xb333, 0x5999);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 25, 0, -8);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(25)->actionFlags |= 1;
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0x26666, 0x13333);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 25, 0, 16);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(25)->actionFlags |= 1;
  Call1((void (*)())Func_02004c9c, 0x85);
  Call3((void (*)())Actor_SetSpeedParameters, 29, 0x26666, 0x13333);
  Actor_GetById(27)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, 0, 3);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = 0;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, 0, 3);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = 0xd000;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, 0, 3);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = 0xb000;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, 0, 3);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = FACING_WEST;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, 0, 3);
  Call1((void (*)())FieldEvent_Wait, 1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = 0x5000;
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x1019, 0, 0xa);
  Call3((void (*)())Func_02004b04, 27, PIXELS(168), PIXELS(424));
  Call2((void (*)())Func_02004c54, 0x1b, 0);
  Actor_SetSingleChildFlags(Actor_GetById(27), 1);
  Actor_GetById(27)->angle = 0xb000;
  Call3((void (*)())Actor_Jump, 27, 6, 40);
  Func_02004b6c_(0x1b, 0x102, 0x28);
  Call3((void (*)())Actor_ArmCallback, 25, 0xd000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Func_02004b6c_(0x19, 0x107, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x1019, 0, 0xa);
  Func_02004b6c_(0x1d, 0x109, 0x32);
  Call3((void (*)())Actor_ArmCallback, 25, 0x3000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Func_02004b6c_(0x19, 0x107, 0x1e);
  Call3((void (*)())Func_02004b4c, 0x1019, 0, 0xa);
  Func_02004b6c_(0x1b, 0x102, 0x32);
  Call2((void (*)())Actor_SetModeById, 25, 5);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Func_02004b4c, 0x1019, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 29, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, 27, 0xd000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Actor_StartRepeatedMotion, 29, 3);
  Call2((void (*)())Func_02004b74, 0x1d, 0x102);
  Call2((void (*)())Actor_StartRepeatedMotion, 27, 3);
  Call2((void (*)())Func_02004b74, 0x1b, 0x102);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Actor_SetSpeedParameters, 29, 0x1cccc, 0xe666);
  Call3((void (*)())Actor_CommitPositionAndActivate, 29, 16, 16);
  Call3((void (*)())Actor_CommitPositionAndActivate, 29, 0, 40);
  Call3((void (*)())Actor_ArmCallback, 29, 0xb000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Actor_SetModeById, 25, 1);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call3((void (*)())Actor_ArmCallback, 29, 0xb000, 0);
  Call3((void (*)())Actor_ArmCallback, 27, 0xb000, 0);
  Call3((void (*)())Func_02004b4c, 0x1019, 0, 0xa);
  Call2((void (*)())Actor_SetModeById, 27, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 29, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_SetSpeedParameters, 25, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 27, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 29, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 28, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 30, 0xcccc, 0x6666);
  Call3((void (*)())Actor_SetSpeedParameters, 31, 0xcccc, 0x6666);
  Call2((void (*)())Camera_FollowActor, 25, 1);
  Call3((void (*)())Actor_CommitPositionAndActivate, 25, -16, 16);
  Call3((void (*)())Actor_CommitPositionAndActivate, 25, 0, 16);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 25, 152, 504);
  Call3((void (*)())Actor_OffsetPositionAndReset, 29, -16, 0);
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, -16, 16);
  Call3((void (*)())Actor_OffsetPositionAndReset, 29, -16, 16);
  Call3((void (*)())Actor_CommitPositionAndActivate, 27, 0, 16);
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 27, 152, 488);
  Call3((void (*)())Actor_SetPositionAndReset, 29, 152, 472);
  Call2((void (*)())Actor_SetModeById, 25, 1);
  Call2((void (*)())Actor_SetModeById, 27, 1);
  Call3((void (*)())Actor_ArmCallback, 29, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 27, 0x5000, 0);
  Call3((void (*)())Actor_ArmCallback, 25, 0x5000, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 4);
  Call3((void (*)())Func_02004b4c, 0x6019, 0, 0xa);
  Call2((void (*)())Actor_StartRepeatedMotion, 4, 3);
  Call2((void (*)())Func_02004b74, 4, 0x102);
  Call2((void (*)())Actor_StartRepeatedMotion, 26, 3);
  Call2((void (*)())Func_02004b74, 0x1a, 0x102);
  Call1((void (*)())FieldEvent_Wait, 40);
  Call3((void (*)())Actor_OffsetPositionAndReset, 27, 0, 64);
  Call3((void (*)())Actor_OffsetPositionAndReset, 29, 0, 64);
  Call3((void (*)())Actor_CommitPositionAndActivate, 25, 0, 64);
  Func_02004b6c_(0x19, 0x106, 0x28);
  Call3((void (*)())Func_02004b4c, 0x2019, 0, 0xa);
  Call3((void (*)())Actor_ArmCallback, 26, 0x3000, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_SOUTHEAST, 0);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x2019, 0, 0xa);
  Call3((void (*)())Actor_CommitPositionAndActivate, 25, 0, 16);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 25, 2);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Actor_StartRepeatedMotion, 30, 2);
  Call2((void (*)())Actor_StartRepeatedMotion, 31, 2);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 28, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b04, 28, PIXELS(176), PIXELS(592));
  Call2((void (*)())Func_02004c54, 0x1c, 0);
  Actor_SetSingleChildFlags(Actor_GetById(28), 1);
  Call3((void (*)())Actor_Jump, 28, 6, 20);
  Call3((void (*)())Actor_ArmCallback, 28, FACING_WEST, 0);
  Call3((void (*)())Func_02004b04, 31, PIXELS(128), PIXELS(608));
  Call2((void (*)())Func_02004c54, 0x1f, 0);
  Actor_SetSingleChildFlags(Actor_GetById(31), 1);
  Call3((void (*)())Actor_Jump, 31, 6, 20);
  Call3((void (*)())Actor_ArmCallback, 31, 0xd000, 0);
  Call3((void (*)())Func_02004b04, 30, PIXELS(192), PIXELS(616));
  Call2((void (*)())Func_02004c54, 0x1e, 0);
  Actor_SetSingleChildFlags(Actor_GetById(30), 1);
  Call3((void (*)())Actor_Jump, 30, 6, 20);
  Call3((void (*)())Actor_ArmCallback, 30, 0xb000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 25, 4);
  Call3((void (*)())Func_02004b4c, 0x19, 0, 0xa);
  Call2((void (*)())Actor_SetModeById, 30, 3);
  Call2((void (*)())Actor_SetModeById, 31, 3);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 28, 3);
  Call1((void (*)())FieldEvent_Wait, 30);
  Call2((void (*)())Actor_WatchAndSetScript, 4, 25);
  Call2((void (*)())Actor_WatchAndSetScript, 26, 25);
  Call2((void (*)())Actor_WatchAndSetScript, 30, 25);
  Call2((void (*)())Actor_WatchAndSetScript, 31, 25);
  Call2((void (*)())Actor_WatchAndSetScript, 28, 25);
  Call2((void (*)())Actor_SetMovementTypeById, 25, (s32)gData_02005670);
  Call2((void (*)())Actor_SetMovementTypeById, 27, (s32)gData_02005670);
  Call1((void (*)())FieldEvent_Wait, 10);
  Call2((void (*)())Actor_SetMovementTypeById, 29, (s32)gData_02005670);
  Call1((void (*)())FieldEvent_Wait, 70);
  Call3((void (*)())Actor_CommitPositionAndActivate, 28, -16, 0);
  Call2((void (*)())Actor_SetMovementTypeById, 28, (s32)gData_02005670);
  Call1((void (*)())FieldEvent_Wait, 33);
  Call3((void (*)())Actor_CommitPositionAndActivate, 31, 16, 0);
  Call2((void (*)())Actor_SetMovementTypeById, 31, (s32)gData_02005670);
  Call1((void (*)())FieldEvent_Wait, 15);
  Call2((void (*)())Actor_SetMovementTypeById, 30, (s32)gData_02005670);
  Call1((void (*)())Actor_RefreshSelectorById, 25);
  Call3((void (*)())Actor_ArmCallback, 26, 0x5000, 0);
  Call1((void (*)())Actor_RefreshSelectorById, 30);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Camera_FollowActor, 4, 1);
  ((void (*)())Func_02004b94)();
  Call1((void (*)())FieldEvent_Wait, 40);
  Func_02004b6c_(0x1a, 0x105, 0x32);
  Call3((void (*)())Actor_ArmCallback, 26, FACING_EAST, 0);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_WEST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0) == 0) {
    Call1((void (*)())FieldEvent_Wait, 20);
    Call3((void (*)())Func_02004b4c, 0x1a, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    Call1((void (*)())FieldEvent_Wait, 40);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Call3((void (*)())Func_02004b4c, 0x1a, 0, 0xa);
  }
  Call3((void (*)())Actor_SetSpeedParameters, 26, 0xcccc, 0x6666);
  Call3((void (*)())Actor_CommitPositionAndActivate, 26, 0, -16);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_NORTH, 0);
  Call3((void (*)())Actor_ArmCallback, 26, 0xd000, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call2((void (*)())Motion_SetVarCbAndRefresh, 26, 2);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Func_02004b4c, 0x1a, 0, 0xa);
  Call3((void (*)())Actor_CommitPositionAndActivate, 26, 0, 16);
  Call3((void (*)())Actor_ArmCallback, 4, FACING_WEST, 0);
  Call3((void (*)())Actor_ArmCallback, 26, FACING_EAST, 0);
  Call1((void (*)())FieldEvent_Wait, 20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (Value2((s32 (*)())Event_AskYesNoWithActorAnimation, 4, 0) != 0) {
    Call1((void (*)())FieldEvent_Wait, 40);
    Call3((void (*)())Func_02004b4c, 0x1a, 0, 0xa);
    Call2((void (*)())Motion_SetModeAndWaitAnimation, 26, 3);
    Call1((void (*)())FieldEvent_Wait, 10);
    Call3((void (*)())Func_02004b4c, 0x1a, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  } else {
    Call1((void (*)())FieldEvent_Wait, 20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
    Call3((void (*)())Func_02004b4c, 0x1a, 0, 0xa);
  }
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 4, 3);
  Call1((void (*)())FieldEvent_Wait, 20);
  Call3((void (*)())Actor_SetSpeedParameters, 26, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 26, 2);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

    if (leader != NULL)
      Call3((void (*)())Actor_ResetAndSetPosition, 26, ((s16 *)leader)[5],
            ((s16 *)leader)[9]);
  }
  Call1((void (*)())Actor_WaitForMoveById, 26);
  Call3((void (*)())Func_02004b04, 26, 0, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  ((void (*)())Func_02004a9c)();
}
