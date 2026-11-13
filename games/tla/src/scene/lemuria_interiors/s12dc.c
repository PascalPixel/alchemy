/// @file
/// Scene code of overlay 0x68c, in ROM order.

#include "../shared/actors.h"
#include "../shared/effects.h"
#include "types.h"
#include "field_event.h"
#include "event_inline.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "field.h"
#include "module/scene/lemuria_interiors.h"
#include "module/scene/scene.h"
#include "module/tla.h"

void Func_0200128c();

DEFINE_ACTOR_EMIT_NOTES(Func_020012dc, Func_0200128c)

DEFINE_ACTOR_CYCLE_PICTURES(Func_02001398)

void Event_Start();
void Event_End();

/// "Honestly, I'm amazed that anyone can travel so far across the world in this
/// age."
void LemuriaInteriors_AskWherePiersIs(void) {
  struct Actor *leader;
  struct Actor *object;
  struct Actor *target;
  s32 result;

  Event_SetFlag(0x969);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors26);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_WalkByAndWait(18, 32, 0);
  Actor_WalkByAndWait(18, 0, -32);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_WalkToAndWait(4, 440, 352);
  Actor_SetPosition(4, PIXELS(440), PIXELS(352));
  FieldActor_LaunchFromLeader(12, 16, 0, FACING_SOUTH + FACING_STEP);
  FieldActor_LaunchFromLeader(5, -16, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(6, -32, 0, FACING_SOUTH);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_WaitForMoveById(6);
  Event_Wait(10);
  FieldCamera_MoveTo(PIXELS(432), -1, PIXELS(368), 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_ShowEmote(18, 0x101, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(6, 0x105, 30);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_ShowEmote(18, 0x109, 40);
  Actor_FaceDirection(18, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_Wait(10);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(18, 0x108, 30);
  Event_OpenMessage(18, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(18, 0, 10);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
  } else {
    Event_Wait(35);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    Event_ShowMessageThenWait(18, 0, 10);
  }
  Actor_ShowEmote(12, 0x101, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, 0x101, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(12, 3);
  Actor_StartRepeatedMotion(18, 3);
  Actor_SetAttachedEffect(18, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(12, 3);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(10);
  Event_Wait(15);
  Actor_ShowEmote(18, 0x102, 40);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirection(4, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Event_Wait(25);
  Actor_ShowEmote(5, 0x102, 40);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_ShowEmote(18, 0x100, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(6, 3);
  Event_Wait(15);
  Actor_SetAnimationAndWait(18, 4);
  Event_ShowMessageThenWait(18, 0, 20);
  Actor_WalkByAndWait(18, 12, 0);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_StartRepeatedMotion(12, 3);
  Actor_SetAttachedEffect(12, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(6, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_SetSpeed(18, 0x20000, 0x10000);
  Actor_WalkByAndWait(18, -12, 0);
  Actor_FaceDirection(18, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ShowEmote(18, 0x102, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(12, 0x109, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, 0x108, 50);
  Actor_FaceDirection(18, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_ShowEmote(12, 0x105, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, 0x101, 45);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(10);
  Actor_SetSpeed(18, 0xcccc, 0x6666);
  Actor_WalkByAndWait(18, 12, 0);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_SetAnimationAndWait(12, 4);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, 0x102, 40);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(10);
  Event_Wait(10);
  Actor_SetAnimationAndWait(18, 4);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_WalkByAndWait(18, -12, 0);
  Actor_FaceDirection(18, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, 0x101, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(5, 0x108, 30);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(18, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(30);
  Func_020012dc(18);
  Event_PlaySound(151);
  Actor_ShowEmote(18, 0x101, 50);
  Actor_FaceDirection(18, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(25);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimationAndWait(5, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_ShowEmote(18, 0x100, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(12, 0x108, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_SetSpeed(18, 0x20000, 0x10000);
  Actor_WalkByAndWait(18, 12, 0);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, 0x107, 30);
  Event_ShowMessageThenWait(0x1012, 0, 10);
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_WalkByAndWait(18, -12, 0);
  Actor_FaceDirection(18, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(10);
  Actor_Jump(18, 6, 15);
  Actor_Jump(18, 6, 23);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(12, 0x109, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  FieldActor_ShowEmote(18, 0x101, 50);
  Actor_SetAnimationAndWait(12, 4);
  Event_ShowMessageThenWait(12, 0, 10);
  FieldActor_ShowEmote(18, 0x102, 50);
  Actor_RunRepeatedMotion(6, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirection(18, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, 0x109, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Event_Wait(20);
  Event_OpenMessage(5, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(20);
    Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(10);
    Event_ShowMessageThenWait(18, 0, 10);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
  } else {
    Event_Wait(35);
    Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    Event_ShowMessageThenWait(18, 0, 10);
  }
  Actor_RunRepeatedMotion(12, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Event_Wait(25);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_OpenMessage(12, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(12, 0, 10);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
  } else {
    Event_Wait(35);
    ((struct EventWork1c4 *)Ram_HeapSlots->eventWork)->count++;
    Event_ShowMessageThenWait(12, 0, 10);
  }
  Actor_ShowEmote(5, 0x101, 0);
  Actor_ShowEmote(6, 0x101, 50);
  Actor_FaceDirection(4, 0x7fff, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(5, 0x7fff, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Event_Wait(40);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 30);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Event_Wait(30);
  Actor_StartRepeatedMotion(6, 3);
  Actor_SetAttachedEffect(6, 0x102);
  Event_Wait(40);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(5, 0x7fff, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Event_Wait(40);
  Actor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, 0x102);
  Actor_StartRepeatedMotion(6, 3);
  Actor_SetAttachedEffect(6, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, 0x101, 50);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(25);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 30);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(12, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetSpeed(12, 0x9999, 0x4ccc);
  Actor_WalkByAndWait(12, 12, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(12, 12, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(6, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(12, 4);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_RunRepeatedMotion(12, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(18, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(18, 3);
  Actor_SetAttachedEffect(18, 0x102);
  Event_Wait(50);
  Actor_ShowEmote(5, 0x101, 30);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(12, 0x8001, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Actor_ShowEmote(6, 0x101, 30);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(20);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(20);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(10);
  Actor_SetSpeed(18, 0x10000, 0x8000);
  Actor_WalkByAndWait(18, 12, 0);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_ShowEmote(4, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 0);
  Actor_ShowEmote(12, 0x100, 45);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, 0x102, 30);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_WalkByAndWait(12, -12, 0);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(15);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_ShowEmote(12, 0x101, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, 0x109, 30);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Event_Wait(40);
  Actor_ShowEmote(12, 0x100, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(25);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(25);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(18, 4);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_FaceDirection(18, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_FaceDirection(6, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  Actor_FaceDirection(4, FACING_SOUTH, 0);
  Actor_FaceDirection(12, FACING_SOUTH + FACING_STEP, 0);
  Event_Wait(30);
  Actor_SetAnimation(4, 4);
  Actor_SetAnimation(5, 4);
  Actor_SetAnimation(6, 4);
  Actor_SetAnimationAndWait(12, 4);
  Actor_SetAnimationAndWait(18, 4);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  FieldActor_ShowEmote(12, 0x101, 50);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_FaceDirection(18, 0xd00a, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_ShowEmote(12, 0x102, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, 0x107, 42);
  Actor_Jump(18, 6, 15);
  Actor_Jump(18, 6, 23);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_WalkByAndWait(18, -2, -12);
  Event_Wait(10);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_SetSpeed(18, 0x19999, 0xcccc);
  Actor_WalkByAndWait(18, -8, 40);
  Actor_FaceDirection(6, FACING_SOUTH, 0);
  Actor_WalkByAndWait(18, 0, 20);
  MapGrid_SetShake(0x10000, 0x10000, 0x10000);
  Actor_SetSpeed(18, 0x1e666, 0xf333);
  Event_PlaySound(133);
  Actor_Jump(18, 2, 0);
  Actor_GetById(18)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(18, 0, -12);
  Event_Wait(1);
  Actor_GetById(18)->actionFlags |= 1;
  MapGrid_SetShake(0, 0, 0);
  Event_Wait(10);
  Actor_ShowEmote(18, 0x102, 30);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_SetSpeed(18, 0x9999, 0x4ccc);
  Actor_GetById(18)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(18, 0, -24);
  Event_Wait(1);
  Actor_GetById(18)->actionFlags |= 1;
  Event_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_Jump(18, 6, 15);
  Actor_Jump(18, 6, 23);
  object = Actor_GetById(18);
  object->timer = 0;
  object->update = (void (*)(void))Func_02001398;
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_SetSpeed(18, 0x23333, 0x11999);
  Actor_WalkByAndWait(18, 0, 32);
  target = Actor_GetById(18);
  Event_PlaySound(134);
  MapGrid_SetShake(0x40000, 0x40000, 0x10000);
  Actor_GetById(18)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(18, 0, -16);
  Event_Wait(1);
  Actor_GetById(18)->actionFlags |= 1;
  target->timer = 3;
  wait(1);
  target->update = 0;
  Actor_SetSpriteFlags((s32)Actor_GetById(18), 1);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x1012, 0, 10);
  Actor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  Actor_StartRepeatedMotion(5, 3);
  Actor_SetAttachedEffect(5, 0x102);
  Actor_StartRepeatedMotion(6, 3);
  Actor_SetAttachedEffect(6, 0x102);
  Actor_StartRepeatedMotion(12, 3);
  Actor_SetAttachedEffect(12, 0x102);
  Event_Wait(40);
  Actor_FaceDirection(18, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(25);
  Event_ShowMessageThenWait(0x1012, 0, 10);
  Actor_WalkByAndWait(18, 0, 48);
  Actor_SetPosition(18, 0, 0);
  Event_Wait(30);
  Actor_ShowEmote(6, 0x105, 30);
  Event_ShowMessageThenWait(6, 0, 10);
  Actor_SetAnimationAndWait(5, 4);
  Event_ShowMessageThenWait(5, 0, 10);
  Actor_Jump(12, 6, 15);
  Actor_Jump(12, 6, 23);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(15);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(4, FACING_EAST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  Actor_FaceDirection(6, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(5, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(6, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetAnimation(12, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(5, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(6, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Event_End();
}

void LemuriaInteriors_TalkToPiersUncle(s32 arg0);
void LemuriaInteriors_MindReadPiersUncle(s32 unused, s32 id);
void LemuriaInteriors_AskAboutDraughts(s32 arg0);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gSceneRegions[] = {
    416, 32, 592, 432, 48, 608, -1, 13,
    688, -24, 608, 704, -8, 624, -1, 14,
    -1, 0, 0, 0, 0, 0, 0, 0,
};
// clang-format on

u32 gSceneExits[] = {
    SCENE_EXITS(203),
    SCENE_EXIT_IF(1, 202, 1, -1),
    SCENE_EXIT_IF(2, 202, 2, -1),
    SCENE_EXIT_IF(3, 202, 3, -1),
    SCENE_EXIT_IF(4, 203, 6, -1),
    SCENE_EXIT_IF(5, 203, 7, -1),
    SCENE_EXIT_IF(6, 203, 4, -1),
    SCENE_EXIT_IF(7, 203, 5, -1),
    SCENE_EXIT_IF(8, 203, 10, -1),
    SCENE_EXIT_IF(9, 203, 11, -1),
    SCENE_EXIT_IF(10, 203, 8, -1),
    SCENE_EXIT_IF(11, 203, 9, -1),
    SCENE_EXIT_IF(12, 202, 12, -1),
    SCENE_EXIT_IF(13, 203, 14, -1),
    SCENE_EXIT_IF(14, 203, 13, -1),
    SCENE_EXIT_IF(15, 202, 15, -1),
    SCENE_EXIT_IF(16, 202, 16, -1),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    {157, CONDITION_ALWAYS, 2, PIXELS(480), 0, PIXELS(144), FACING_SOUTH, 0, 0},
    {156, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(136), FACING_NORTH + FACING_STEP, 1, 0},
    {160, CONDITION_ALWAYS, 1, PIXELS(96), 0, PIXELS(104), FACING_WEST, 0, 0},
    {219, CONDITION_ALWAYS, 1, PIXELS(64), 0, PIXELS(88), FACING_EAST, 0, 0},
    {56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(328), FACING_NORTHWEST + FACING_STEP, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(588), FACING_NORTHWEST + FACING_STEP, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(552), FACING_SOUTH + FACING_STEP, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(40), 0, PIXELS(328), FACING_SOUTHEAST + FACING_STEP, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(80), 0, PIXELS(552), FACING_SOUTHEAST + FACING_STEP, 3, 0},
    {43, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(416), FACING_NORTH + FACING_STEP, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(96), 0, PIXELS(792), FACING_NORTHWEST + FACING_STEP, 1, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x4401, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 30, 0x966, EVENT(LemuriaInteriors_LeavePiersToMourn)},
    {TRIGGER_STEP, 31, 0x969, EVENT(LemuriaInteriors_AskWherePiersIs)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors01)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors02)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors03)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors04)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(LemuriaInteriors_TalkToPiersUncle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, EVENT(LemuriaInteriors_MindReadPiersUncle)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(LemuriaInteriors_AskFromBeyondSeaOfTime)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors14)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors15)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors16)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors17)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(LemuriaInteriors_AskAboutDraughts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1037, 0x967, EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1038, 0x967, EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1039, 0x967, EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1040, 0x967, EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1041, 0x967, EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1043, 0x967, EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors21)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors22)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors23)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors25)},
    {243, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors27))},
    {243, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors28))},
    {243, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors29))},
    {243, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors30))},
    {243, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors31))},
    {243, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors32))},
    {243, 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors33))},
    {243, 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors34))},
    {451, 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors35))},
    {TRIGGERS_END},
};

u32 gData_02002dec[] = {
    38,
};
