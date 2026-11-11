#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02004a94();
void Func_02004a9c();
void Func_02004b94();

void Func_0200112c(void) {
  struct Actor *object0;
  struct Actor *object1;
  struct Actor *object2;

  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgItSIt);
  FieldCamera_SetSpeed(0x40000, 0x8000);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  Actor_WalkToAndWait(4, 232, 392);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x2000);
  FieldActor_LaunchFromLeader(5, 0, 0, 0x2000);
  FieldActor_LaunchFromLeader(6, 0, 16, 0x2000);
  Actor_WalkByAndWait(4, 16, 0);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  FieldCamera_MoveToActor(22, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(22, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldCamera_MoveToActor(23, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  FieldActor_Jump(23, 6, 15);
  FieldActor_Jump(23, 6, 23);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(22, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(24, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(25, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(11, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  FieldCamera_MoveToActor(24, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(24, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(25, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  Actor_FaceDirection(25, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(25, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  Actor_FaceDirection(22, FACING_EAST, 0);
  Actor_FaceDirection(23, 0xd000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(14, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_MoveToActor(22, 1);
  Func_02004b94();
  FieldEvent_Wait(10);
  Actor_ShowEmote(22, 0x107, 40);
  FieldEvent_ShowMessageThenWait(22, 0, 10);
  FieldCamera_MoveTo(PIXELS(488), 0, PIXELS(416), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Actor_ShowEmote(23, 0x102, 50);
  Actor_FaceDirection(23, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(23, 0, 10);
  Actor_FaceDirection(24, FACING_EAST, 0);
  Actor_FaceDirection(25, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(25, 3);
  Actor_SetAttachedEffect(25, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(25, 0, 10);
  FieldCamera_MoveTo(PIXELS(392), PIXELS(16), PIXELS(416), 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(24, 4);
  FieldEvent_ShowMessageThenWait(24, 0, 10);
  Actor_FaceDirection(23, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(22, 0x105, 0);
  Actor_ShowEmote(23, 0x105, 0);
  Actor_ShowEmote(25, 0x105, 50);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(25, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(25, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(25, 0, 20);
  Actor_SetAnimation(22, 3);
  Actor_SetAnimation(23, 3);
  Actor_SetAnimationAndWait(24, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(22, FACING_EAST, 0);
  Actor_FaceDirection(23, 0x5000, 0);
  Actor_FaceDirection(24, FACING_NORTH, 0);
  Actor_FaceDirection(25, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToActor(gMainRoutineWork.currentOwner, 1);
  Func_02004b94();
  FieldEvent_Wait(20);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object0 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object0 == 0)
    goto L_020094c6;
  Actor_SetDestination(5, *(s16 *)((u8 *)object0 + 10), *(s16 *)((u8 *)object0 + 18));
L_020094c6:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object1 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object1 == 0)
    goto L_02009504;
  Actor_SetDestination(6, *(s16 *)((u8 *)object1 + 10), *(s16 *)((u8 *)object1 + 18));
L_02009504:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  object2 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object2 == 0)
    goto L_02009550;
  Actor_SetDestination(26, *(s16 *)((u8 *)object2 + 10), *(s16 *)((u8 *)object2 + 18));
L_02009550:
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_02004b94();
  Func_02004a9c();
  return;
}

void Func_02001574(void) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;

  result0 = Event_GetFlag(0x8ab);
  if (result0 != 0)
    goto L_02009586;
  goto L_020097dc;
L_02009586:
  result1 = Event_GetFlag(0x8a7);
  if (result1 == 0)
    goto L_02009596;
  goto L_020097dc;
L_02009596:
  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgYouDoPlan);
  Event_SetFlag(0x8ac);
  Actor_WalkToAndWait(4, 264, 448);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x4000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0x8000);
  FieldActor_LaunchFromLeader(6, 16, -16, 0x6000);
  result2 = Event_GetFlag(7);
  if (result2 == 0)
    goto L_02009604;
  FieldActor_LaunchFromLeader(7, 8, 16, 0xc000);
L_02009604:
  FieldActor_WaitForMoveById(6);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x102, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(5, FACING_WEST, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(30);
  Actor_ShowEmote(26, 0x106, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  result3 = Event_GetFlag(7);
  if (result3 == 0)
    goto L_02009716;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  object4 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object4 == 0)
    goto L_02009706;
  Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                            *(s16 *)((u8 *)object4 + 18));
L_02009706:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_02009716:
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object5 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object5 == 0)
    goto L_0200974c;
  Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                            *(s16 *)((u8 *)object5 + 18));
L_0200974c:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object6 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object6 == 0)
    goto L_0200978a;
  Actor_SetDestination(6, *(s16 *)((u8 *)object6 + 10),
                            *(s16 *)((u8 *)object6 + 18));
L_0200978a:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  object7 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object7 == 0)
    goto L_020097c8;
  Actor_SetDestination(26, *(s16 *)((u8 *)object7 + 10),
                            *(s16 *)((u8 *)object7 + 18));
L_020097c8:
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  Func_02004a9c();
L_020097dc:
  return;
}

void Func_020017ec(void) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;
  struct Actor *object4;
  struct Actor *object5;
  struct Actor *object6;
  struct Actor *object7;

  Func_02004a94();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgAreYouFinished);
  Actor_WalkToAndWait(4, 232, 392);
  FieldActor_LaunchFromLeader(26, 0, -16, 0x4000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0x8000);
  FieldActor_LaunchFromLeader(6, 16, -16, 0x6000);
  result0 = Event_GetFlag(7);
  if (result0 == 0)
    goto L_02009850;
  FieldActor_LaunchFromLeader(7, 8, 16, 0xc000);
L_02009850:
  FieldActor_WaitForMoveById(6);
  Actor_FaceDirection(4, 0xc000, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(26, 2);
  FieldEvent_Wait(10);
  Event_OpenMessage(26, 0);
  result1 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result1 != 0)
    goto L_020098ea;
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x100, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  goto L_02009938;
L_020098ea:
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
  FieldEvent_Wait(40);
  Actor_SetAnimationAndWait(6, 3);
  Actor_FaceDirection(4, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
L_02009938:
  result2 = ((s32 (*)())GameFlag_Test)(7);
  if (result2 == 0)
    goto L_0200996c;
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirectionAndWait(6, 0x8000);
  FieldEvent_Wait(10);
  goto L_02009a2e;
L_0200996c:
  ((s32 (*)())Func_080d47b4)(26, 0x102, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(26, 0x101, 30);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  ((s32 (*)())Func_080d47b4)(26, 0x109, 30);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, 0x4000, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(26, 4);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
L_02009a2e:
  FieldEvent_SetMessageId((s32)MsgSoIThink);
  Actor_SetAnimationAndWait(26, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(26, 0, 10);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(10);
  result3 = Event_GetFlag(7);
  if (result3 == 0)
    goto L_02009ac2;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  object4 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object4 == 0)
    goto L_02009ab2;
  Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                            *(s16 *)((u8 *)object4 + 18));
L_02009ab2:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_02009ac2:
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object5 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object5 == 0)
    goto L_02009af8;
  Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                            *(s16 *)((u8 *)object5 + 18));
L_02009af8:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object6 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object6 == 0)
    goto L_02009b36;
  Actor_SetDestination(6, *(s16 *)((u8 *)object6 + 10),
                            *(s16 *)((u8 *)object6 + 18));
L_02009b36:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(26, 0x13333, 0x9999);
  Actor_SetAnimation(26, 2);
  object7 = Actor_GetById(gMainRoutineWork.currentOwner);
  if (object7 == 0)
    goto L_02009b74;
  Actor_SetDestination(26, *(s16 *)((u8 *)object7 + 10),
                            *(s16 *)((u8 *)object7 + 18));
L_02009b74:
  FieldActor_WaitForMoveById(26);
  Actor_SetPosition(26, 0, 0);
  Func_02004a9c();
  return;
}

struct SpawnObject {
  u8 unk00[0x50];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[8];
  u16 unk5e;
};

extern const u8 gData_020056d4[];

void Func_02004a0c(struct SpawnObject *object, s32 mode);
void Func_02004a14(struct SpawnObject *object, const void *script);
void Func_02004a74(struct SpawnObject *object, s32 priority);

/// Spawns object type 0x1e8 at x, y, z with its sprite turned to value and
/// its timer set.
void Func_02001b9c(s32 x, s32 y, s32 z, s16 value, s32 timer) {
  struct SpawnObject *object;
  struct FieldSprite *sprite;

  object = ((struct SpawnObject *)FieldActor_Create(0x1e8, x, y, z));
  if (object != 0) {
    sprite = object->sprite;
    Func_02004a0c(object, 1);
    Func_02004a14(object, gData_020056d4);
    Func_02004a74(object, 1);
    object->motionFlags = 0;
    {
      /* FAKEMATCH(signature): the timer copied into an s64 local, so it loads
       * before the pooled zero for nextFrame; as object->unk5e = timer, 2 of 35
       * instructions differ in tla-en. */
      s64 wide = timer;

      object->unk5e = wide;
    }
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}
