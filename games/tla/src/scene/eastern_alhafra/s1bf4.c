/// @file
/// Scene code of overlay 0x668, in ROM order, from s1bf4's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "event_inline.h"

s32 Func_02001b9c(s32 x, s32 y, s32 z, s32 rotation, s32 timer);

void Func_02001bf4(s32 id) {
  struct Actor *object = Actor_Get(id);

  Func_02001b9c(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
  Func_02001b9c(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
  Func_02001b9c(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
  Func_02001b9c(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                15);
  Event_PlaySound(151);
  FieldEvent_Wait(15);
}

#include "scene_tables.h"
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

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Event_StartScreenTransition();
void Func_02001bf4();
void Event_FinishScreenTransition();
void Event_WaitTransitionFrames();
void Func_02004a94();
void Func_02004a9c();
void Func_02004b94();
void Func_02004ba4();

static __inline__ void Func_02004b6c_(s32 actor, s32 a, s32 b) {
  Func_080d47b4(actor, a, b);
}

static __inline__ void FieldMessage_ShowAtActor_(s32 actor, s32 flags) {
  s32 f = flags;

  FieldMessage_ShowAtActor(actor, f);
}

/// The scene that opens with MsgHeyThatBlock: either answer to the prompt
/// steps the event to its next message.
void Func_02001cb0(void) {
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgHeyThatBlock);
  Actor_SetSpeed(28, 0x2cccc, 0x16666);
  Actor_SetSpeed(29, 0x2cccc, 0x16666);
  Actor_SetSpeed(23, 0x1cccc, 0xe666);
  Actor_SetSpeed(24, 0x1cccc, 0xe666);
  Actor_SetPosition(28, PIXELS(328), PIXELS(616));
  Actor_FaceDirection(28, FACING_NORTHEAST, 0);
  Actor_SetPosition(29, PIXELS(296), PIXELS(616));
  Actor_FaceDirection(29, FACING_NORTHEAST, 0);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  {
    s32 mode = 1;

    FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(632), mode);
  }
  Func_02004b94();
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c, 0x102);
  FieldEvent_Wait(50);
  Func_02004b6c_(0x1d, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_FaceDirection(28, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(28, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_FaceDirection(29, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_FaceDirection(29, FACING_WEST, 0);
  FieldEvent_Wait(10);
  Actor_WalkBy(28, -160, 0);
  Actor_WalkByAndWait(29, -160, 0);
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  FieldEvent_Wait(30);
  Actor_SetPosition(23, PIXELS(264), PIXELS(384));
  Actor_FaceDirection(23, FACING_EAST, 0);
  Actor_SetPosition(24, PIXELS(280), PIXELS(392));
  Actor_FaceDirection(24, FACING_EAST, 0);
  Actor_SetPosition(4, PIXELS(232), PIXELS(384));
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x18, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  FieldCamera_MoveTo(PIXELS(264), -1, PIXELS(392), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(23, -16, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x17, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(23, 3);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    FieldEvent_Wait(40);
    Actor_SetAnimationAndWait(23, 4);
    FieldEvent_Wait(10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  }
  Actor_FaceDirection(23, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(24, FACING_WEST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Func_02004b6c_(0x18, 0x108, 0x2d);
  FieldActor_Jump(24, 6, 15);
  FieldActor_Jump(24, 6, 23);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(23, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(23, 0, 8);
  FieldActor_WatchAndSetScript(4, 23);
  Actor_WalkBy(24, -96, 0);
  Actor_WalkByAndWait(23, -96, 0);
  Actor_WalkBy(23, 0, 80);
  Actor_WalkByAndWait(24, -32, 0);
  Actor_WalkByAndWait(24, 0, 80);
  FieldActor_Stop(4);
  Actor_SetPosition(24, 0, 0);
  Actor_SetPosition(23, 0, 0);
  FieldCamera_FollowActor(4, 1);
  Func_02004b94();
  Func_02004a9c();
}

/// Only once flag 0x8a7 is set: the scene that opens with MsgIStillJust, which
/// sets flag 0x905 and ends by sending the party to SceneId_Ov667, entrance 81.
void Func_02002048(void) {
  if (Event_GetFlag(0x8a7) == 0)
    return;
  Event_SetFlag(0x905);
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgIStillJust);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  Actor_SetPosition(25, PIXELS(56), PIXELS(632));
  Actor_FaceDirection(25, FACING_EAST, 0);
  Actor_SetPosition(27, PIXELS(56), PIXELS(648));
  Actor_FaceDirection(27, FACING_EAST, 0);
  Actor_SetPosition(28, PIXELS(48), PIXELS(616));
  Actor_FaceDirection(28, FACING_EAST, 0);
  Actor_SetPosition(29, PIXELS(32), PIXELS(608));
  Actor_FaceDirection(29, FACING_EAST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_MoveTo(PIXELS(144), -1, PIXELS(664), 1);
  FieldEvent_Wait(10);
  Actor_SetSpeed(28, 0x10000, 0x8000);
  Actor_SetSpeed(29, 0x10000, 0x8000);
  Actor_SetSpeed(25, 0x10000, 0x8000);
  Actor_SetSpeed(27, 0x10000, 0x8000);
  Actor_WalkBy(25, 32, 0);
  Actor_WalkBy(27, 32, 0);
  Actor_WalkBy(28, 24, 24);
  Actor_WalkBy(29, 24, 24);
  FieldActor_WaitForMoveById(28);
  FieldActor_WaitForMoveById(29);
  Actor_FaceDirection(28, FACING_EAST, 0);
  Actor_FaceDirection(29, FACING_EAST, 0);
  Func_02004b94();
  FieldEvent_Wait(10);
  Func_02004b6c_(0x1c, 0x107, 0);
  Func_02004b6c_(0x1c, 0x107, 0x28);
  Actor_SetSpeed(28, 0x30000, 0x18000);
  Actor_SetSpeed(29, 0x30000, 0x18000);
  Actor_SetScript(28, gData_020054f4);
  Actor_SetScript(29, gData_0200556c);
  FieldActor_RefreshSelectorById(28);
  FieldActor_RefreshSelectorById(29);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  FieldActor_Jump(29, 6, 15);
  FieldActor_Jump(29, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_SetSpeed(28, 0x19999, 0xcccc);
  Actor_SetSpeed(29, 0x19999, 0xcccc);
  Actor_SetSpeed(25, 0x19999, 0xcccc);
  Actor_SetSpeed(27, 0x19999, 0xcccc);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(0x19, 0x102);
  FieldActor_StartRepeatedMotion(27, 3);
  Actor_SetAttachedEffect(0x1b, 0x102);
  FieldEvent_Wait(40);
  Actor_SetScript(28, gData_0200d5d0);
  Actor_SetScript(29, gData_02005620);
  FieldEvent_Wait(20);
  FieldCamera_FollowActor(25, 1);
  Actor_WalkBy(25, 64, 0);
  Actor_WalkByAndWait(27, 64, 0);
  Actor_WalkTo(25, 168, 616);
  Actor_WalkToAndWait(27, 184, 616);
  FieldActor_RefreshSelectorById(25);
  Actor_SetAnimation(25, 1);
  Actor_FaceDirection(25, FACING_NORTHEAST, 0);
  Actor_FaceDirection(27, FACING_NORTHEAST, 0);
  FieldActor_RefreshSelectorById(28);
  FieldActor_RefreshSelectorById(29);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(0x19, 0x102);
  FieldActor_StartRepeatedMotion(27, 3);
  Actor_SetAttachedEffect(0x1b, 0x102);
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(0x60000, 0xc000);
  FieldCamera_MoveTo(PIXELS(384), -1, PIXELS(480), 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_SetSpeed(0x60000, 0xc000);
  FieldCamera_MoveTo(PIXELS(480), -1, PIXELS(344), 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_SetSpeed(0x60000, 0xc000);
  FieldCamera_MoveTo(PIXELS(488), -1, PIXELS(456), 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_SetSpeed(0x60000, 0xc000);
  FieldCamera_MoveTo(PIXELS(192), -1, PIXELS(616), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Func_02004b6c_(0x19, 0x102, 0x1e);
  Actor_FaceDirection(25, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_FaceDirection(28, 0x3000, 0);
  Actor_FaceDirection(29, 0x5000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(28, 3);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(27, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x1b, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0xa);
  Actor_FaceDirection(25, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimation(28, 3);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(25, 0xd000, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  FieldEvent_Wait(40);
  Func_02004b6c_(0x19, 0x101, 0x1e);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldActor_StartRepeatedMotion(28, 3);
  Actor_SetAttachedEffect(0x1c, 0x102);
  FieldActor_StartRepeatedMotion(29, 3);
  Actor_SetAttachedEffect(0x1d, 0x102);
  FieldEvent_Wait(50);
  Actor_FaceDirection(28, 0xb000, 0);
  Actor_FaceDirection(29, 0xb000, 0);
  FieldEvent_Wait(40);
  Func_02004b6c_(0x1c, 0x100, 0);
  Func_02004b6c_(0x1d, 0x100, 0x28);
  Actor_FaceDirection(25, 0xb000, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(PIXELS(144), -1, PIXELS(544), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Func_02004b6c_(4, 0x102, 0x1e);
  Actor_SetSpeed(25, 0x18000, 0xc000);
  Actor_SetSpeed(27, 0x18000, 0xc000);
  Actor_WalkBy(25, -32, -32);
  Actor_WalkByAndWait(27, -32, -32);
  Actor_WalkBy(25, 0, -24);
  Actor_WalkByAndWait(27, 0, -24);
  FieldActor_WaitForMoveById(25);
  Actor_FaceDirection(25, 0xd000, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x19, 0x100, 0x1e);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_WalkToAndWait(4, 136, 520);
  FieldActor_LaunchFromLeader(0x1a, 0x10, 0, 0x5000);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  FieldActor_WaitForMoveById(26);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
  Func_02004b6c_(0x19, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_FaceDirection(26, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(26, 0x5000, 0);
  Actor_FaceDirection(27, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0xa);
  Func_02004b6c_(0x19, 0x100, 0x1e);
  Actor_FaceDirection(25, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_FaceDirection(27, 0x3000, 0);
  Actor_FaceDirection(25, 0x3000, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(25, 6, 15);
  FieldActor_Jump(25, 6, 23);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1c, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(29, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0x14);
  Actor_WalkBy(28, 0, 64);
  Actor_WalkBy(29, 0, 64);
  FieldEvent_Wait(60);
  Actor_SetPosition(28, 0, 0);
  Actor_SetPosition(29, 0, 0);
  Actor_FaceDirection(27, 0xb000, 0);
  Actor_FaceDirection(25, 0xd000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldMessage_ShowAtActor_(0x19, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(40);
    Actor_FaceDirection(26, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(26, 4);
    FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    FieldEvent_Wait(30);
    Actor_FaceDirection(26, FACING_WEST, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimationAndWait(26, 3);
    FieldEvent_Wait(10);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    FieldEvent_ShowMessageThenWait(0x201a, 0, 0xa);
  }
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(30);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(26, 0x3000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(25, 0x3000, 0);
  Actor_FaceDirection(27, 0x3000, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(4, 0xcccc, 0x6666);
  Actor_SetSpeed(26, 0xcccc, 0x6666);
  Actor_SetSpeed(25, 0xcccc, 0x6666);
  Actor_SetSpeed(27, 0xcccc, 0x6666);
  Actor_GetById(4)->motionFlags = 0;
  Actor_GetById(26)->motionFlags = 0;
  Actor_GetById(25)->motionFlags = 0;
  Actor_GetById(27)->motionFlags = 0;
  Actor_WalkBy(4, 0, 120);
  Actor_WalkBy(26, 0, 120);
  Actor_WalkBy(27, 0, 80);
  Actor_WalkByAndWait(25, 0, 80);
  {
    struct EventWork *event = Ram_HeapSlots->eventWork;

    event->delay = 60;
    event->screenEffect = 0x100;
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Call2((void (*)())Func_02004ba4, (s32)&SceneId_Ov667, 81);
  Func_02004a9c();
}

/// Only once flag 0x8a7 is set: the scene that opens with MsgBriggs2, which sets
/// flags 0x90f and 0x97f and ends by sending the party to SceneId_Ov668Room1,
/// entrance 99.
void Func_02002a08(void) {
  struct MapWork *map;
  s32 i;
  s32 step;

  if (Event_GetFlag(0x8a7) == 0)
    return;
  Event_SetFlag(0x90f);
  Event_SetFlag(0x97f);
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgBriggs2);
  FieldEvent_ShowMessageThenWait(0x1d, 0, 0xa);
  Actor_WalkToAndWait(4, 136, 552);
  FieldActor_LaunchFromLeader(0x1a, -0x10, 0, 0xa000);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  FieldActor_WaitForMoveById(26);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x1a, 0x100, 0x1e);
  FieldCamera_MoveTo(PIXELS(176), -1, PIXELS(392), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1b, 0, 0xa);
  Actor_SetPosition(22, PIXELS(224), PIXELS(8));
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Ram_HeapSlots->mapWork->minX = 0;
  Ram_HeapSlots->mapWork->maxX = 0x4000000;
  Ram_HeapSlots->mapWork->minZ = 0;
  Ram_HeapSlots->mapWork->maxZ = 0x4000000;
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldCamera_MoveTo(PIXELS(208), -1, PIXELS(168), 1);
  FieldEvent_Wait(50);
  Event_PlaySound(0x4e);
  Event_FinishScreenTransition();
  PaletteFade_Interpolate(0x10);
  Event_WaitTransitionFrames();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(664), 0);
  {
    struct Actor *center =
        ((struct EventWork *)Ram_HeapSlots->eventWork)->viewCenter;

    FieldActor_SetPositionAndResetMotion(center, center->x, center->y,
                                    center->z);
  }
  FieldEvent_Wait(50);
  FieldActor_SetSpritePriority(22, 1);
  FieldActor_SetSpritePriority(23, 1);
  Actor_SetSpeed(23, 0x13333, 0x9999);
  Actor_SetSpeed(22, 0x13333, 0x9999);
  Actor_SetPosition(22, PIXELS(792), PIXELS(856));
  Actor_SetPosition(23, PIXELS(824), PIXELS(856));
  Actor_SetPosition(26, PIXELS(744), PIXELS(1016));
  Actor_SetPosition(25, PIXELS(744), PIXELS(1016));
  Event_PlaySound(0x23);
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(30);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  FieldCamera_MoveTo(PIXELS(808), -1, PIXELS(848), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 23);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Actor_SetAnimationAndWait(23, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_ArmCallback(22, FACING_EAST, 0);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(25);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(23, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(22, 0x5000, 0);
  Actor_FaceDirection(23, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  Func_02004b6c_(0x17, 0x100, 0);
  Func_02004b6c_(0x16, 0x100, 0x32);
  FieldActor_StartRepeatedMotion(22, 3);
  Actor_SetAttachedEffect(0x16, 0x102);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(22, 0, 16);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x16, 0x102, 0x1e);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Actor_WalkByAndWait(23, 0, 16);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x17, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_FaceDirection(23, 0x5000, 0);
  FieldEvent_Wait(50);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x16, 0x101, 0x2d);
  Actor_WalkByAndWait(22, 0, 32);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x16, 0x106, 0x2d);
  Actor_WalkByAndWait(22, 0, -32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Actor_FaceDirection(23, 0x5000, 0);
  FieldEvent_Wait(50);
  Actor_ArmCallback(23, FACING_WEST, 0);
  FieldEvent_Wait(20);
  Func_02004b6c_(0x17, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Func_02004b6c_(0x16, 0x101, 0x32);
  Actor_WalkByAndWait(22, 0, 32);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x16, 0x106, 0x32);
  Actor_WalkByAndWait(22, 0, -32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Func_02004b6c_(0x17, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_FaceDirection(22, 0x5000, 0);
  FieldEvent_Wait(30);
  Func_02001bf4(0x16);
  Event_PlaySound(0x67);
  Func_02004b6c_(0x16, 0x105, 0x32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(23, 4);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_FaceDirection(22, 0x5000, 0);
  FieldEvent_Wait(30);
  Func_02001bf4(0x16);
  Func_02004b6c_(0x16, 0x106, 0x32);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Func_02004b6c_(0x17, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(22, 0x5000);
  Actor_SetAnimation(22, 7);
  FieldEvent_Wait(20);
  Actor_SetAnimation(22, 6);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Func_02004b6c_(0x16, 0x102, 0x32);
  Actor_SetAnimation(22, 7);
  FieldEvent_Wait(20);
  Actor_SetAnimation(22, 1);
  Actor_ArmCallback(22, FACING_EAST, 0);
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Func_02004b6c_(0x17, 0x103, 0x1e);
  FieldEvent_ShowMessageThenWait(0x17, 0, 0xa);
  Func_02004b6c_(0x16, 0x106, 0x28);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  Actor_WalkByAndWait(22, 0, 8);
  Actor_WalkByAndWait(22, 10, 16);
  Actor_FaceDirection(23, 0x5000, 0);
  Actor_WalkByAndWait(22, 6, 16);
  Actor_ArmCallback(22, 0x5000, 0);
  FieldEvent_Wait(10);
  FieldActor_Jump(22, 6, 15);
  FieldActor_Jump(22, 6, 23);
  FieldEvent_ShowMessageThenWait(0x4016, 0, 0xa);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  Func_02004b6c_(0x16, 0x108, 0x1e);
  FieldEvent_ShowMessageThenWait(0x4016, 0, 0x14);
  Actor_SetAnimation(22, 8);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0xa);
  FieldActor_Jump(22, 7, 0);
  i = 15;
turn:
  // The angle, through s16: as the u16 field its halfword zero is shared with
  // the motion-flag stores below and loads from the pool (differ 19 more).
  ((s16 *)Actor_GetById(22))[3] -= 0x1000;
  i--;
  System_Wait(1);
  if (i >= 0)
    goto turn;
  Actor_SetAnimation(22, 5);
  FieldEvent_Wait(7);
  FieldEvent_ShowMessageThenWait(0x16, 0, 0x14);
  Func_02004b6c_(0x17, 0x108, 0x32);
  Actor_WalkByAndWait(23, 0, 36);
  FieldEvent_Wait(15);
  Actor_SetAnimation(22, 1);
  FieldEvent_Wait(20);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(0x4017, 0, 0xf);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(22, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimation(22, 9);
  FieldEvent_Wait(70);
  map = Ram_HeapSlots->mapWork;
  Actor_GetById(22)->motionFlags = 0;
  Actor_GetById(23)->motionFlags = 0;
  MapGrid_CopyCellsLayered(42, 60, 17, 4, 42, 64);
  MapGrid_CopyCellsLayered(42, 60, 17, 4, 42, 68);
  MapGrid_CopyCellsLayered(106, 60, 17, 4, 106, 64);
  MapGrid_CopyCellsLayered(106, 60, 17, 4, 106, 68);
  /* FAKEMATCH(temporary): the step held in a local, used for the first layer
   * and both actors, gives the native's registers; as the literal each time, 14
   * of 853 instructions differ in tla-en. */
  step = 0x10000;
  i = 239;
scroll:
  *(s32 *)(map->layers[0].unk08 + 4) += step;
  *(s32 *)(map->layers[1].unk08 + 4) += 0x10000;
  Actor_GetById(22)->z -= step;
  i--;
  Actor_GetById(23)->z -= step;
  System_Wait(1);
  if (i >= 0)
    goto scroll;
  Event_PlaySound(0x4e);
  FieldEvent_Wait(30);
  Func_02004a9c();
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Func_02004ba4((s32)&SceneId_Ov668Room1, 99);
}

/// The scene that opens with MsgWhatYouLet; either answer to each of its two
/// prompts steps the event's message number, by 1 and then by 2.
void Func_02003274(void) {
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhatYouLet);
  FieldCamera_MoveTo(PIXELS(152), -1, PIXELS(272), 0);
  {
    struct Actor *center =
        ((struct EventWork *)Ram_HeapSlots->eventWork)->viewCenter;

    FieldActor_SetPositionAndResetMotion(center, center->x, center->y,
                                    center->z);
  }
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldCamera_MoveTo(PIXELS(152), -1, PIXELS(392), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Func_02004b6c_(0x19, 0x103, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_FaceDirection(25, 0x5000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  FieldActor_Jump(25, 6, 15);
  FieldActor_Jump(25, 6, 23);
  FieldEvent_Wait(10);
  Func_02004b6c_(0x19, 0x109, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_FaceDirection(25, 0xd000, 0);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x19, 0x100, 0x1e);
  Actor_SetSpeed(25, 0xb333, 0x5999);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, 8);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(25, 0x26666, 0x13333);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, -16);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkBy(25, 0, 8);
  Event_PlaySound(0x85);
  Actor_SetSpeed(29, 0x26666, 0x13333);
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = FACING_WEST;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = 0xb000;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = 0xd000;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = 0;
  Actor_GetById(29)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(29, 0, -4);
  FieldEvent_Wait(1);
  Actor_GetById(29)->actionFlags |= 1;
  Actor_GetById(29)->angle = 0x3000;
  FieldEvent_Wait(20);
  Actor_GetById(25)->actionFlags |= 1;
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_SetPosition(29, PIXELS(168), PIXELS(368));
  Actor_SetSpriteRotationById(0x1d, 0);
  Actor_SetSingleChildFlags(Actor_GetById(29), 1);
  Actor_FaceDirection(29, 0x5000, 0);
  FieldActor_Jump(29, 6, 40);
  Func_02004b6c_(0x1d, 0x102, 0x28);
  Actor_ArmCallback(25, 0x3000, 0);
  FieldEvent_Wait(30);
  Actor_SetSpeed(25, 0xb333, 0x5999);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, -8);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  FieldEvent_Wait(20);
  Actor_SetSpeed(25, 0x26666, 0x13333);
  Actor_GetById(25)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(25, 0, 16);
  FieldEvent_Wait(1);
  Actor_GetById(25)->actionFlags |= 1;
  Event_PlaySound(0x85);
  Actor_SetSpeed(29, 0x26666, 0x13333);
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = 0;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = 0xd000;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = 0xb000;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = FACING_WEST;
  Actor_GetById(27)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(27, 0, 3);
  FieldEvent_Wait(1);
  Actor_GetById(27)->actionFlags |= 1;
  Actor_GetById(27)->angle = 0x5000;
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_SetPosition(27, PIXELS(168), PIXELS(424));
  Actor_SetSpriteRotationById(0x1b, 0);
  Actor_SetSingleChildFlags(Actor_GetById(27), 1);
  Actor_GetById(27)->angle = 0xb000;
  FieldActor_Jump(27, 6, 40);
  Func_02004b6c_(0x1b, 0x102, 0x28);
  Actor_ArmCallback(25, 0xd000, 0);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x19, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Func_02004b6c_(0x1d, 0x109, 0x32);
  Actor_ArmCallback(25, 0x3000, 0);
  FieldEvent_Wait(30);
  Func_02004b6c_(0x19, 0x107, 0x1e);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Func_02004b6c_(0x1b, 0x102, 0x32);
  Actor_SetAnimation(25, 5);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_ArmCallback(29, 0x3000, 0);
  Actor_ArmCallback(27, 0xd000, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(29, 3);
  Actor_SetAttachedEffect(0x1d, 0x102);
  FieldActor_StartRepeatedMotion(27, 3);
  Actor_SetAttachedEffect(0x1b, 0x102);
  FieldEvent_Wait(40);
  Actor_SetSpeed(29, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(29, 16, 16);
  Actor_WalkByAndWait(29, 0, 40);
  Actor_ArmCallback(29, 0xb000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimation(25, 1);
  FieldEvent_Wait(30);
  Actor_ArmCallback(29, 0xb000, 0);
  Actor_ArmCallback(27, 0xb000, 0);
  FieldEvent_ShowMessageThenWait(0x1019, 0, 0xa);
  Actor_SetAnimation(27, 3);
  Actor_SetAnimationAndWait(29, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(25, 0xcccc, 0x6666);
  Actor_SetSpeed(27, 0xcccc, 0x6666);
  Actor_SetSpeed(29, 0xcccc, 0x6666);
  Actor_SetSpeed(28, 0xcccc, 0x6666);
  Actor_SetSpeed(30, 0xcccc, 0x6666);
  Actor_SetSpeed(31, 0xcccc, 0x6666);
  FieldCamera_FollowActor(25, 1);
  Actor_WalkByAndWait(25, -16, 16);
  Actor_WalkByAndWait(25, 0, 16);
  Actor_WalkTo(25, 152, 504);
  Actor_WalkBy(29, -16, 0);
  Actor_WalkByAndWait(27, -16, 16);
  Actor_WalkBy(29, -16, 16);
  Actor_WalkByAndWait(27, 0, 16);
  Actor_WalkTo(27, 152, 488);
  Actor_WalkToAndWait(29, 152, 472);
  Actor_SetAnimation(25, 1);
  Actor_SetAnimation(27, 1);
  Actor_ArmCallback(29, 0x5000, 0);
  Actor_ArmCallback(27, 0x5000, 0);
  Actor_ArmCallback(25, 0x5000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 4);
  FieldEvent_ShowMessageThenWait(0x6019, 0, 0xa);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldActor_StartRepeatedMotion(26, 3);
  Actor_SetAttachedEffect(0x1a, 0x102);
  FieldEvent_Wait(40);
  Actor_WalkBy(27, 0, 64);
  Actor_WalkBy(29, 0, 64);
  Actor_WalkByAndWait(25, 0, 64);
  Func_02004b6c_(0x19, 0x106, 0x28);
  FieldEvent_ShowMessageThenWait(0x2019, 0, 0xa);
  Actor_ArmCallback(26, 0x3000, 0);
  Actor_ArmCallback(4, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2019, 0, 0xa);
  Actor_WalkByAndWait(25, 0, 16);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  FieldActor_StartRepeatedMotion(30, 2);
  FieldActor_StartRepeatedMotion(31, 2);
  Actor_RunRepeatedMotion(28, 2);
  FieldEvent_Wait(20);
  Actor_SetPosition(28, PIXELS(176), PIXELS(592));
  Actor_SetSpriteRotationById(0x1c, 0);
  Actor_SetSingleChildFlags(Actor_GetById(28), 1);
  FieldActor_Jump(28, 6, 20);
  Actor_ArmCallback(28, FACING_WEST, 0);
  Actor_SetPosition(31, PIXELS(128), PIXELS(608));
  Actor_SetSpriteRotationById(0x1f, 0);
  Actor_SetSingleChildFlags(Actor_GetById(31), 1);
  FieldActor_Jump(31, 6, 20);
  Actor_ArmCallback(31, 0xd000, 0);
  Actor_SetPosition(30, PIXELS(192), PIXELS(616));
  Actor_SetSpriteRotationById(0x1e, 0);
  Actor_SetSingleChildFlags(Actor_GetById(30), 1);
  FieldActor_Jump(30, 6, 20);
  Actor_ArmCallback(30, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(25, 4);
  FieldEvent_ShowMessageThenWait(0x19, 0, 0xa);
  Actor_SetAnimation(30, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimationAndWait(28, 3);
  FieldEvent_Wait(30);
  FieldActor_WatchAndSetScript(4, 25);
  FieldActor_WatchAndSetScript(26, 25);
  FieldActor_WatchAndSetScript(30, 25);
  FieldActor_WatchAndSetScript(31, 25);
  FieldActor_WatchAndSetScript(28, 25);
  Actor_SetScript(25, (s32)gData_02005670);
  Actor_SetScript(27, (s32)gData_02005670);
  FieldEvent_Wait(10);
  Actor_SetScript(29, (s32)gData_02005670);
  FieldEvent_Wait(70);
  Actor_WalkByAndWait(28, -16, 0);
  Actor_SetScript(28, (s32)gData_02005670);
  FieldEvent_Wait(33);
  Actor_WalkByAndWait(31, 16, 0);
  Actor_SetScript(31, (s32)gData_02005670);
  FieldEvent_Wait(15);
  Actor_SetScript(30, (s32)gData_02005670);
  FieldActor_RefreshSelectorById(25);
  Actor_ArmCallback(26, 0x5000, 0);
  FieldActor_RefreshSelectorById(30);
  FieldEvent_Wait(20);
  FieldCamera_FollowActor(4, 1);
  Func_02004b94();
  FieldEvent_Wait(40);
  Func_02004b6c_(0x1a, 0x105, 0x32);
  Actor_ArmCallback(26, FACING_EAST, 0);
  Actor_ArmCallback(4, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(20);
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    FieldEvent_Wait(40);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  }
  Actor_SetSpeed(26, 0xcccc, 0x6666);
  Actor_WalkByAndWait(26, 0, -16);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_ArmCallback(26, 0xd000, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  Actor_WalkByAndWait(26, 0, 16);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_ArmCallback(26, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldMessage_ShowAtActor_(0x1a, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0) {
    FieldEvent_Wait(40);
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
    Actor_SetAnimationAndWait(26, 3);
    FieldEvent_Wait(10);
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  } else {
    FieldEvent_Wait(20);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
    FieldEvent_ShowMessageThenWait(0x1a, 0, 0xa);
  }
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

    if (leader != NULL)
      Actor_SetDestination(26, ((s16 *)leader)[5], ((s16 *)leader)[9]);
  }
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  FieldEvent_Wait(10);
  Func_02004a9c();
}

#include "system/ram_buffer.h"

struct Particle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 age;
};

struct ParticleSprite {
  u8 unk0[8];
  u16 attr;
  u8 unkA[30];
};

struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

void Func_02004c6c(struct ParticleSprite *sprite, struct Particle *particle);
void Func_02004c74(struct Particle *particle, s32 unk1, s32 step);

void Func_02003ec4(void) {
  struct ParticleWork *work = Ram_HeapSlots->menuRuntime;
  struct Particle *particle = work->particles;
  struct ParticleSprite *sprite = work->sprites;
  s32 i;
  s32 step = -0x8000;

  i = 63;
next:
  if ((u32)particle->age <= 19) {
    u16 *base = &work->tile;
    s32 mask = 7;
    s32 tile = (u16)((*base + ((particle->age >> 2) & mask) * 8) & 0x3ff);
    s32 attr = sprite->attr;

    sprite->attr = (attr & ~0x3ff) | tile;
    Func_02004c6c(sprite, particle);
    Func_02004c74(particle, 63, step);
    particle->age++;
  }
  i--;
  sprite++;
  particle++;
  if (i >= 0)
    goto next;
}

/// One falling rock: where it is, how it moves and how long it has fallen.
struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

/// A falling rock's sprite.
struct RockSprite {
  u8 unk00[5];
  u8 unk05;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

s32 Random16Far(void);

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
void Func_02003f48(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work;
  s32 i;

  /* FAKEMATCH(barrier): an empty asm before the work is read keeps the spills
   * of x and y at the entry; as an initializer, 4 of 88 instructions differ in
   * tla-en. */
  asm("");
  work = Ram_HeapSlots->menuRuntime;
  // A goto loop carries no loop notes, so the offsets are rebuilt on every
  // pass, as the bytes have them; a for loop hoists them.
  if (count > 0) {
    i = count;
  loop:;
    {
      s32 angle = Random16Far();
      struct Rock *rock = &work->rocks[work->next];

      rock->timer = 0;
      rock->position[0] = x;
      rock->position[1] = y;
      rock->position[2] = z;
      FieldVector_AddPolarOffset((Random16Far() << 3) + 0x80000, angle,
                               rock->position);
      rock->velocity[0] = 0;
      rock->velocity[1] = 0x50000;
      rock->velocity[2] = 0;
      FieldVector_AddPolarOffset((Random16Far() << 1) + 0x20000, angle,
                               rock->velocity);
      work->next++;
      work->next &= 63;
    }
    i--;
    if (i != 0)
      goto loop;
  }
}

