#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "event_inline.h"

void Animation_SetIndexAndInitializeObjects();
void Func_020001bc();
void Func_02001bcc();
void Func_02001bd4();

void Func_02000e7c(s32 id) {
  struct Actor *leader;
  s32 a;
  s32 b;
  s32 c;

  Func_02001bcc();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgOh2);
  Animation_SetIndexAndInitializeObjects((s32)Actor_Get(12), 56);
  if (id == 21) {
    Actor_WalkToAndWait(4, 376, 968);
    Actor_FaceDirection(4, FACING_EAST, 0);
    FieldActor_LaunchFromLeader(12, 16, -16, FACING_EAST);
    a = 0;
    b = 0x800a;
    c = 0x5000;
  } else {
    Actor_WalkToAndWait(4, 552, 968);
    Actor_FaceDirection(4, FACING_WEST, 0);
    FieldActor_LaunchFromLeader(12, -16, -16, FACING_WEST);
    a = 0x8000;
    b = -10;
    c = 0x3000;
  }
  FieldActor_WaitForMoveById(12);
  FieldActor_WatchAndSetScript(4, 12);
  Actor_SetSpeed(12, 0x10000, 0x8000);
  FieldCamera_FollowActor(12, 1);
  Actor_WalkToAndWait(12, 464, 952);
  Actor_FaceDirectionAndWait(12, a);
  FieldEvent_Wait(10);
  Actor_ShowEmote(12, 0x100, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_FaceDirectionAndWait(12, b);
  FieldEvent_Wait(10);
  Actor_ShowEmote(12, 0x100, 30);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_WalkToAndWait(12, 464, 784);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  FieldActor_Jump(12, 6, 15);
  FieldActor_Jump(12, 6, 23);
  FieldEvent_Wait(10);
  Actor_FaceDirectionAndWait(12, c);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  FieldEvent_Wait(20);
  FieldActor_SetSpritePriority(4, 1);
  Actor_SetPosition(4, PIXELS(464), PIXELS(856));
  Actor_WalkToAndWait(4, 464, 796);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(12, 3);
  FieldEvent_Wait(10);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetAnimation(12, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  FieldEvent_Wait(10);
  Actor_Get(4)->priorityFlags |= 1;
  Actor_WalkToAndWait(4, 464, 776);
  Func_020001bc();
  Func_02001bd4();
}

void Func_0200109c(void) {
  struct Actor *leader;

  Func_02001bcc();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgWhereAreYou);
  Animation_SetIndexAndInitializeObjects((s32)Actor_Get(12), 56);
  Actor_FaceDirectionAndWait(4, FACING_SOUTH);
  FieldActor_SetSpritePriority(12, 1);
  FieldActor_LaunchFromLeader(12, 0, 16, FACING_NORTH + FACING_STEP);
  FieldActor_WaitForMoveById(12);
  FieldEvent_Wait(5);
  FieldEvent_ShowMessageThenWait(12, 0, 5);
  Actor_SetSpeed(12, 0x13333, 0x9999);
  Actor_SetAnimation(12, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1],
                              ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(4, 0, -16);
  Func_020001bc();
  Func_02001bd4();
}
