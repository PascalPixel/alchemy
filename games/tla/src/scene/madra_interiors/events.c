#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02001d50();
void Func_02001d58();
void Func_02001ef8();
void Func_02001910(void);
void Func_02001a20(void);
void Func_02001d50();
void Func_02001d58();
void PartyInventory_Remove();
extern u32 gData_02001f18[];
extern u32 gData_02001f4c[];
extern u32 gData_02001f90[];
extern u32 gData_02001fc4[];
extern u32 gData_02002054[];
extern u32 gData_020020e4[];

void Func_02001008(void) {
  struct Actor *object;

  Event_SetFlag(0x98a);
  Event_SetFlag(0x310);
  PartyInventory_Remove(0x1c0);
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSpeed(11, 0x6666, 0x3333);
  Actor_WalkTo(11, 410, 109);
  object = Actor_Get(4);
  if (object->z > 0x5f0000)
    Actor_WalkToAndWait(4, object->x / 0x10000, 95);
  Actor_WalkToAndWait(4, 362, 95);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldActor_WaitForMoveById(11);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors110);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x102, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_ShowEmote(11, 0x100, 3);
  Actor_WalkToAndWait(11, 378, 109);
  Actor_FaceDirection(11, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_FaceEachOther(10, 11, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x108, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(11, 0x102, 30);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_FaceActor(10, 11, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(30);
  Actor_ShowEmote(10, 0x101, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Actor_FaceDirectionAndWait(11, FACING_SOUTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(11, FACING_NORTH);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(11, FACING_WEST);
  FieldEvent_Wait(10);
  Actor_ShowEmote(11, 0x106, 30);
  FieldActor_Stop(25);
  Actor_SetSpeed(10, 0x8000, 0x4000);
  Actor_SetSpeed(11, 0x8000, 0x4000);
  Actor_SetSpeed(25, 0x8000, 0x4000);
  Actor_SetScript(11, (s32)gData_02001f18);
  Actor_SetScript(4, (s32)gData_02001f4c);
  FieldEvent_Wait(60);
  Actor_SetScript(10, (s32)gData_02001f90);
  FieldActor_RefreshSelectorById(10);
  FieldEvent_Wait(120);
  Actor_SetScript(11, (s32)gData_02001fc4);
  FieldActor_WatchAndSetScript(4, 11);
  FieldEvent_Wait(60);
  Actor_SetScript(10, (s32)gData_02002054);
  Actor_SetScript(25, (s32)gData_020020e4);
  FieldActor_RefreshSelectorById(10);
  FieldActor_RefreshSelectorById(11);
  FieldActor_RefreshSelectorById(25);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Djinn_JoinParty(25, 2, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  Actor_SetAnimation(4, 1);
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(10, 0, 10);
  Actor_SetAnimationAndWait(11, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 10);
  Func_02001d58();
}

void Func_02001310(void) {
  struct Actor *object0;

  Event_SetFlag(0x210);
  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  object0 = Actor_Get(4);
  if (object0->z > 0x5f0000)
    Actor_WalkToAndWait(4, object0->x / 0x10000, 95);
  Actor_WalkToAndWait(4, 362, 95);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors110);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(10, 0x102, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 5);
  Actor_ShowEmote(11, 0x100, 3);
  Actor_FaceActor(11, 4, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors115);
  Actor_ShowEmote(10, 0x100, 30);
  Actor_FaceDirectionAndWait(10, FACING_EAST);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 5);
  Actor_FaceEachOther(10, 11, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(10, 4, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x105, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 5);
  Actor_FaceActor(11, 4, 0);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(11, 2);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(11, 0, 5);
  Func_02001d58();
  return;
}

void Func_02001450(void) {
  s32 result0;
  s32 result1;
  struct Actor *object2;
  struct Actor *object3;
  struct Actor *object4;
  struct Actor *object5;

  Func_02001d50();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgMadraInteriors126);
  Actor_ShowEmote(28, 0x100, 30);
  Actor_FaceActor(28, 4, 0);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceActor(15, 4, 0);
  Actor_FaceDirection(17, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceActor(18, 4, 0);
  Actor_FaceActor(14, 4, 0);
  Actor_FaceActor(15, 4, 0);
  Actor_FaceActor(16, 4, 0);
  FieldEvent_Wait(20);
  Actor_WalkToAndWait(4, 701, 93);
  Actor_WalkBy(4, -32, 0);
  FieldActor_LaunchFromLeader(5, -8, 32, FACING_WEST);
  FieldActor_LaunchFromLeader(31, -16, 0, FACING_SOUTH + FACING_STEP);
  FieldActor_LaunchFromLeader(6, -8, 16, FACING_SOUTHWEST);
  FieldActor_LaunchFromLeader(7, -24, 16, FACING_SOUTHWEST);
  FieldEvent_Wait(30);
  Actor_ShowEmote(15, 0x100, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_ShowEmote(27, 0x100, 30);
  FieldEvent_Wait(20);
  Actor_FaceDirection(27, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(27, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(27, 0x109, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceEachOther(7, 4, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(7, FACING_WEST, 0);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_Jump(18, 6, 20);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST + FACING_STEP);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(28, 0x102, 30);
  Event_OpenMessage(28, 0);
  result0 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result0 != 0)
    goto L_02009690;
  Actor_ShowEmote(27, 0x108, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(15, 0x108, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 3;
  goto L_02009700;
L_02009690:
  Actor_ShowEmote(15, 0x102, 30);
  Func_02001ef8();
  ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 3;
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  FieldActor_StartRepeatedMotion(15, 3);
  Actor_SetAttachedEffect(15, 0x102);
  FieldEvent_Wait(50);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
L_02009700:
  Actor_SetAnimation(7, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  result1 = Event_GetFlag(0x981);
  if (result1 == 0)
    goto L_02009760;
  Func_02001910();
  goto L_02009764;
L_02009760:
  Func_02001a20();
L_02009764:
  Actor_FaceDirection(7, FACING_SOUTH, 0);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(40);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(31, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(4, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(7, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(31, 3);
  Actor_SetAnimation(7, 3);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_SetSpeed(31, 0x13333, 0x9999);
  Actor_SetAnimation(31, 2);
  object2 = Actor_Get(4);
  if (object2 == 0)
    goto L_02009832;
  Actor_SetDestination(31, *(s16 *)((u8 *)object2 + 10),
                            *(s16 *)((u8 *)object2 + 18));
L_02009832:
  FieldActor_WaitForMoveById(31);
  Actor_SetPosition(31, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  object3 = Actor_Get(4);
  if (object3 == 0)
    goto L_02009870;
  Actor_SetDestination(6, *(s16 *)((u8 *)object3 + 10),
                            *(s16 *)((u8 *)object3 + 18));
L_02009870:
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  object4 = Actor_Get(4);
  if (object4 == 0)
    goto L_020098ae;
  Actor_SetDestination(7, *(s16 *)((u8 *)object4 + 10),
                            *(s16 *)((u8 *)object4 + 18));
L_020098ae:
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  object5 = Actor_Get(4);
  if (object5 == 0)
    goto L_020098ec;
  Actor_SetDestination(5, *(s16 *)((u8 *)object5 + 10),
                            *(s16 *)((u8 *)object5 + 18));
L_020098ec:
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Event_SetFlag(0x989);
  Func_02001d58();
  return;
}

void Func_02001910(void) {
  FieldEvent_SetMessageId((s32)MsgMadraInteriors127);
  FieldEvent_ShowMessageThenWait(28, 0, 10);
  Actor_FaceEachOther(4, 31, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(31, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(4, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(15, 0x100, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(27, 0x102, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_ShowEmote(5, 0x108, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  return;
}

void Func_02001a20(void) {
  FieldEvent_SetMessageId((s32)MsgMadraInteriors128);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetAnimationAndWait(5, 4);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_FaceActor(6, 5, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_NORTH);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_ShowEmote(17, 0x101, 30);
  Actor_FaceActor(7, 17, 0);
  Actor_ShowEmote(7, 0x108, 30);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(31, 0x102, 30);
  Actor_FaceActor(31, 7, 0);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_FaceActor(7, 31, 0);
  FieldEvent_Wait(60);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceActor(4, 7, 0);
  Actor_FaceActor(6, 7, 0);
  Actor_FaceActor(5, 7, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x101, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 10);
  Actor_FaceActor(7, 6, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(4, 0x101, 30);
  Actor_FaceActor(7, 4, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_ShowEmote(5, 0x109, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(7, 4);
  FieldEvent_Wait(20);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHEAST);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_FaceActor(7, 31, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  Actor_FaceActor(7, 4, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Event_SetFlag(0x981);
  Actor_ShowEmote(15, 0x100, 30);
  FieldEvent_ShowMessageThenWait(15, 0, 10);
  Actor_FaceDirectionAndWait(7, FACING_SOUTHWEST);
  Actor_SetAnimationAndWait(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(7, 0, 10);
  Actor_FaceDirection(4, FACING_SOUTHWEST, 0);
  Actor_ShowEmote(27, 0x102, 30);
  FieldEvent_ShowMessageThenWait(27, 0, 10);
  Actor_SetAnimationAndWait(31, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(31, 0, 10);
  Actor_ShowEmote(5, 0x108, 30);
  Actor_FaceDirection(5, FACING_WEST, 0);
  FieldEvent_ShowMessageThenWait(5, 0, 10);
  Actor_SetAnimationAndWait(17, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(17, 0, 10);
  Actor_FaceDirectionAndWait(5, FACING_SOUTH);
  Actor_SetAnimationAndWait(5, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(18, 0, 10);
  return;
}
