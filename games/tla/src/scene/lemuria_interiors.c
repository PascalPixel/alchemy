#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"
#include "version.h"
#include "module/scene/lemuria_interiors.h"
#include "module/scene/scene.h"
#include "module/tla.h"

void Event_Start();
void Event_End();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) {
  return (const struct SceneRegion *)gSceneRegions;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000058(s32 arg0) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;

  result0 = Event_GetFlag(0x966);
  if (result0 == 0)
    goto L_02008082;
  Event_SetMessageId((s32)MsgLemuriaInteriors11);
  Event_ShowMessageAndWait(arg0, 0);
  Actor_WatchAndSetScript(10, 4);
  goto L_020080f6;
L_02008082:
  result1 = Event_GetFlag(0x965);
  if (result1 == 0)
    goto L_020080ba;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors08);
  Event_ShowMessageAndWait(arg0, 0);
  Actor_FaceDirection(arg0, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_End();
  goto L_020080f6;
L_020080ba:
  Event_SetMessageId((s32)MsgLemuriaInteriors05);
  FieldMessage_ShowAtActor(arg0, 0);
  result2 = FieldParty_GetLeaderActor();
  result3 = FieldEvent_AskYesNoWithActorAnimation(result2, 0);
  if (result3 != 0)
    goto L_020080dc;
  Func_0200031c();
  goto L_020080f6;
L_020080dc:
  Event_Wait(20);
  Event_ShowMessageAndWait(arg0, 0);
  Actor_FaceDirection(arg0, FACING_WEST, 0);
L_020080f6:
  return;
}

void Func_02000104(s32 unused, s32 id) {
  if (GameFlag_Test(0x966)) {
    Event_SetMessageId((s32)MsgLemuriaInteriors12);
    Event_ShowMessageAndWait(id, 0);
  } else if (GameFlag_Test(0x965)) {
    Event_SetMessageId((s32)MsgLemuriaInteriors09);
    Event_ShowMessageAndWait(id, 0);
  } else {
    Event_SetMessageId((s32)MsgLemuriaInteriors06);
    Event_ShowMessageAndWait(id, 0);
  }
}

void Func_02000154(s32 arg0) {
  s32 result0;
  s32 result1;

  Event_SetMessageId((s32)MsgLemuriaInteriors13);
  FieldMessage_ShowAtActor(arg0, 0);
  result0 = FieldParty_GetLeaderActor();
  result1 = FieldEvent_AskYesNoWithActorAnimation(result0, 0);
  if (result1 != 0)
    goto L_02008184;
  Event_Wait(10);
  Event_SetMessageId(((s32)MsgLemuriaInteriors13 + 1));
  goto L_02008190;
L_02008184:
  Event_Wait(20);
  Event_SetMessageId(((s32)MsgLemuriaInteriors13 + 2));
L_02008190:
  Event_ShowMessageAndWait(arg0, 0);
  return;
}

void Func_020001a0(s32 arg0) {
  s32 result0;
  s32 result1;

  Event_SetMessageId((s32)MsgLemuriaInteriors18);
  FieldMessage_ShowAtActor(arg0, 0);
  result0 = FieldParty_GetLeaderActor();
  result1 = FieldEvent_AskYesNoWithActorAnimation(result0, 0);
  if (result1 != 0)
    goto L_020081d0;
  Event_Wait(10);
  Event_SetMessageId(((s32)MsgLemuriaInteriors18 + 1));
  goto L_020081dc;
L_020081d0:
  Event_Wait(20);
  Event_SetMessageId(((s32)MsgLemuriaInteriors18 + 2));
L_020081dc:
  Event_ShowMessageAndWait(arg0, 0);
  return;
}

void Func_020001ec(s32 arg0, s32 arg1) {
  struct Actor *object0;
  s32 value1;
  s32 result2;
  s32 result3;

  object0 = Actor_GetById(arg1);
  value1 = object0->facing;
  GameFlag_SetBit(0x967);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmote(arg1, 0x101, 30);
  Actor_SetAngleToward(arg1, 4, 20);
  Event_SetMessageId((s32)MsgLemuriaInteriors19);
  FieldMessage_ShowAtActor(arg1, 0);
  result2 = FieldParty_GetLeaderActor();
  result3 = FieldEvent_AskYesNoWithActorAnimation(result2, 0);
  if (result3 != 0)
    goto L_0200824e;
  Event_Wait(10);
  Event_SetMessageId(((s32)MsgLemuriaInteriors19 + 1));
  goto L_0200825a;
L_0200824e:
  Event_Wait(20);
  Event_SetMessageId(((s32)MsgLemuriaInteriors19 + 2));
L_0200825a:
  Event_ShowMessageAndWait(arg1, 0);
  Actor_ArmCallback(arg1, value1, 0);
  Event_Wait(10);
  Event_End();
  return;
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/* VIEW(lemuria_interiors): routine Party_RemoveMember as void Party_RemoveMember(); module/game/game.h has s32 Party_RemoveMember(s32 owner). */
extern void Party_RemoveMember__lemuria_interiors() __asm__("Party_RemoveMember");

s32 Scene_Initialize(void) {
  struct Actor *object;

#if !TLA_PATCH_1
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x201;
#else
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
#endif
  Actor_SetSpritePriority(11, 1);
  Actor_SetSpritePriority(14, 2);
  Actor_SetSpritePriority(10, 2);
  object = Actor_GetById(15);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  object = Actor_GetById(17);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  if (GameFlag_Test(0x966)) {
    Actor_SetPositionAndReset(10, 100, 112);
    Actor_GetById(10)->facing = FACING_SOUTHEAST + FACING_STEP;
    wait(1);
  }
  if (GameFlag_Test(0x969))
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200031c(void) {
  struct Actor *object0;
  struct Actor *object1;
  s32 result2;
  struct Actor *object3;

  GameFlag_SetBit(0x965);
  Party_RemoveMember__lemuria_interiors(7);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors07);
  Event_Wait(20);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetPositionAndReset(4, 112, 128);
  FieldActor_LaunchFromLeader(12, 16, 0, FACING_NORTHWEST + FACING_STEP);
  FieldActor_LaunchFromLeader(7, 16, -16, FACING_WEST);
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Actor_WaitForMoveById(7);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(10, 0x100, 30);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Event_Wait(20);
  Actor_Jump(10, 6, 15);
  Actor_Jump(10, 6, 23);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, 0x100, 45);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x200c, 0, 10);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(10, 0x108, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetModeById(12, 3);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Event_Wait(45);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(10, 0x102, 40);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(10);
  Actor_ArmCallback(10, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_Wait(30);
  Actor_ShowEmote(7, 0x100, 45);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Event_Wait(35);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_Wait(35);
  Event_ShowMessageThenWait(7, 0, 10);
  Motion_SetVarCbAndRefresh(10, 2);
  Event_Wait(20);
  Actor_ShowEmote(10, 0x105, 60);
  Actor_CommitPositionAndActivate(7, 48, 0);
  Event_Wait(10);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
  Event_Wait(40);
  Actor_CommitPositionAndActivate(7, -24, 0);
  Event_Wait(10);
  Actor_FaceDirection(7, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ArmCallback(7, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(7, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_ShowEmote(7, 0x101, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(7, 0x102, 40);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(12, 0x100, 30);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(12, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(0x200c, 0, 10);
  Motion_SetVarCbAndRefresh(7, 2);
  Event_Wait(20);
  Actor_ArmCallback(7, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(7, 0x109, 30);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(50);
  Actor_FaceDirection(12, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ArmCallback(4, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(7, 3);
  Actor_SetAttachedEffect(7, 0x102);
  Event_Wait(50);
  Actor_CommitPositionAndActivate(7, -12, 0);
  Event_Wait(10);
  Actor_ShowEmote(10, 0x109, 30);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetVarCbAndRefresh(7, 2);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Motion_SetModeAndWaitAnimation(7, 4);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(10, 4);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_ArmCallback(10, FACING_NORTHWEST + FACING_STEP, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(10, 0, 20);
  Motion_SetModeAndWaitAnimation(10, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(10, 0, 10);
  Event_Wait(10);
  Actor_SetModeById(7, 9);
  Event_Wait(40);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_ArmCallback(12, FACING_NORTH + FACING_STEP, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(7, 0, 10);
  Actor_ArmCallback(10, FACING_EAST, 0);
  Event_Wait(20);
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_SetModeById(7, 1);
  Event_Wait(30);
  Actor_SetSpeedParameters(7, 0x4ccc, 0x2666);
  object0 = Actor_GetById(7);
  object0->actionFlags &= 254;
  Actor_CommitPositionAndActivate(7, 12, 0);
  Event_Wait(1);
  object1 = Actor_GetById(7);
  object1->actionFlags |= 1;
  Event_Wait(10);
  Actor_StartRepeatedMotion(7, 3);
  FieldEffect_SetAttached(7, 0x102);
  Event_Wait(55);
  Actor_WatchAndSetScript(4, 7);
  Actor_WatchAndSetScript(12, 7);
  Actor_WatchAndSetScript(10, 7);
  Actor_SetSpeed(7, 0x23333, 0x11999);
  Actor_CommitPositionAndActivate(7, -8, 88);
  Actor_Stop(4);
  Actor_Stop(12);
  Actor_Stop(10);
  Actor_SetHorizontalPositionWithTerrain(7, 0, 0);
  Event_Wait(30);
  Actor_ShowEmote(12, 0x109, 30);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_ArmCallback(4, FACING_EAST, 0);
  Event_Wait(20);
  Event_OpenMessage(0x200c, 0);
  result2 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result2 != 0)
    goto L_020089be;
  Event_Wait(20);
  Event_ShowMessageThenWait(0x200c, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  goto L_020089e4;
L_020089be:
  Event_Wait(40);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Event_ShowMessageThenWait(0x200c, 0, 10);
L_020089e4:
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(4, 3);
  Event_Wait(20);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetModeById(12, 2);
  object3 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object3 == 0)
    goto L_02008a36;
  Actor_ResetAndSetPosition(12, *(s16 *)((u8 *)object3 + 10),
                            *(s16 *)((u8 *)object3 + 18));
L_02008a36:
  Actor_WaitForMoveById(12);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Event_End();
  return;
}

#include "iwram_call.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02000a5c(s32 a0, s32 a1, s32 a2) {
  struct Actor *actor;
  s32 startBuf[3], targetBuf[3], posBuf[3];
  s32 *start, *target, *pos;
  s32 radius, angle;
  s32 result, i, phase, sine, answer;

  actor = (struct Actor *)((s32)Actor_GetById(11));
  result = GameFlag_Test(0x965);
  if (result != 0)
    goto L_02008a86;
  return;
L_02008a86:
  Event_SetFlag(0x966);
  {
    struct Actor *other = (struct Actor *)((s32)Actor_GetById(10));
    other->priorityFlags |= 1;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors10);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_ShowEmote(10, 0x100, 30);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_WalkToAndWait(4, 136, 152);
  Actor_FaceDirection(4, 0xa000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(10, 4);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_ShowEmote(10, 0x109, 30);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, 0x2000, 0);
  FieldEvent_Wait(30);
  Actor_WalkByAndWait(4, 8, 16);
  Actor_FaceDirection(4, 0x4000, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_ShowEmote(4, 0x100, 30);
  Actor_FaceDirection(4, 0xa000, 0);
  FieldEvent_Wait(30);
  Actor_WalkByAndWait(4, -8, -16);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_SetAnimationAndWait(10, 4);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(50);
  Actor_WalkByAndWait(10, 12, 12);
  FieldEvent_Wait(10);
  Actor_ShowEmote(10, 0x101, 30);
  Event_OpenMessage(10, 0);
  answer = Event_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  } else {
    FieldEvent_Wait(40);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  }
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, 0x3000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(50);
  Actor_ShowEmote(10, 0x101, 30);
  Event_OpenMessage(10, 0);
  answer = Event_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
  } else {
    FieldEvent_Wait(40);
    FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452) =
        (FIELD_AT(((s32)Ram_HeapSlots->eventWork), u16, 452)) + 1;
    FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  }
  Actor_ShowEmote(10, 0x102, 40);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 20);
  Actor_ShowEmote(10, 0x106, 40);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_WalkToAndWait(10, 89, 92);
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, 0x8000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_FaceDirection(10, 0x2000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(10, 4);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_FaceDirection(10, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(10, 0x108, 50);
  Actor_SetAnimation(10, 3);
  FieldEvent_Wait(10);
  Event_PlaySound(0x26b);
  FieldEvent_Wait(60);
  FieldActor_StartRepeatedMotion(11, 3);
  Actor_SetAttachedEffect(11, 0x102);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(11, 3);
  Actor_SetAttachedEffect(11, 0x102);
  FieldEvent_Wait(50);
  Actor_SetAnimation(11, 3);
  actor->motionFlags = 0;
  start = startBuf;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target = targetBuf;
  target[0] = 0x740000;
  target[1] = 0;
  target[2] = 0x6e0000;
  angle = 0xb000;
  for (i = 0; i <= 191; i++) {
    phase = i * 32768 / 192;
    radius = Math_Sine(phase) * 32;
    angle += 512;
    pos = posBuf;
    pos[0] = start[0] + (target[0] - start[0]) * i / 192;
    sine = Math_Sine(phase);
    pos[1] = start[1] + (target[1] - start[1]) * i / 192 + sine * 56;
    pos[2] = start[2] + (target[2] - start[2]) * i / 192;
    Vector_AddPolarOffsetFar(radius, angle, pos);
    actor->x = pos[0];
    actor->y = pos[1];
    actor->z = pos[2];
    actor->facing = angle + 16384;
    if (i == 128) {
      Actor_FaceDirection(10, 0x3000, 0);
    }
    System_Wait(1);
  }
  actor->facing = 0x8000;
  FieldEvent_Wait(30);
  Actor_SetAnimation(11, 1);
  FieldEvent_Wait(20);
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_WalkToAndWait(10, 100, 112);
  Actor_FaceDirection(10, 0, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(10, 0x6666, 0x3333);
  Actor_WalkByAndWait(10, 8, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(11, 3);
  FieldEvent_Wait(40);
  Actor_SetAnimation(11, 1);
  FieldEvent_Wait(20);
  {
    struct Actor *other = (struct Actor *)((s32)Actor_GetById(10));
    other->actionFlags &= 254;
  }
  Actor_WalkByAndWait(10, -8, 0);
  FieldEvent_Wait(1);
  {
    struct Actor *other = (struct Actor *)((s32)Actor_GetById(10));
    other->actionFlags |= 1;
  }
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, 0x3000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  Actor_FaceDirection(10, 0, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x108, 40);
  Actor_SetAnimation(11, 3);
  FieldEvent_Wait(40);
  actor->motionFlags = 0;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target[0] = 0x840000;
  target[1] = 0x200000;
  target[2] = 0x640000;
  angle = 0x4200;
  for (i = 0; i <= 191; i++) {
    radius = Math_Sine(i * 16384 / 192) * 40;
    angle += 768;
    pos[0] = start[0] + (target[0] - start[0]) * i / 192;
    sine = Math_Sine(i * 32768 / 192);
    pos[1] = start[1] + (target[1] - start[1]) * i / 192 + (sine <<= 5);
    pos[2] = start[2] + (target[2] - start[2]) * i / 192;
    Vector_AddPolarOffsetFar(radius, angle, pos);
    actor->x = pos[0];
    actor->y = pos[1];
    actor->z = pos[2];
    actor->facing = angle + 16384;
    if (i == 153) {
      Actor_FaceDirection(10, 0x7000, 0);
    }
    System_Wait(1);
  }
  Actor_FaceDirection(10, 0xb000, 0);
  actor->facing = 0xd000;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target[0] = 0x5a0000;
  target[1] = 0x40000;
  target[2] = 0x400000;
  for (i = 0; i <= 15; i++) {
    actor->x = start[0] + (target[0] - start[0]) * i / 16;
    actor->y = start[1] + (target[1] - start[1]) * i / 16;
    actor->z = start[2] + (target[2] - start[2]) * i / 16;
    System_Wait(1);
  }
  Actor_SetPosition(11, 0, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(10, 0x3000, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x400a, 0, 10);
  FieldActor_SetSpritePriority(10, 2);
  Event_End();
}

#include "field_event.h"

/* VIEW(lemuria_interiors): routine Func_020027f4 as void Func_020027f4(struct Actor *object, s32 mode); module/scene/kandorean_temple.h has void Func_020027f4(void). */
extern void Func_020027f4__lemuria_interiors(struct Actor *object, s32 mode) __asm__("Func_020027f4");
void Actor_StartScript(struct Actor *object, const void *script);

void Func_0200128c(s32 x, s32 y, s32 z, s16 rotation, s32 timer) {
  struct Actor *object;
  struct AnimatedSprite *sprite;

  object = Import_ActorCreate(0x1e8, x, y, z);
  if (object != 0) {
    sprite = (struct AnimatedSprite *)object->sprite;
    Func_020027f4__lemuria_interiors(object, 1);
    Actor_StartScript(object, gData_02002dec);
    object->motionFlags = 0;
    {
      /* FAKEMATCH(barrier): the empty asm before the timer's store; as no asm,
       * 2 of 32 instructions differ in tla-en. */
      __asm__("");
      ((struct SpawnedActor *)object)->timer = timer;
    }
    sprite->nextPicture = 0;
    sprite->rotation = rotation;
  }
}
