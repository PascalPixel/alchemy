#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

struct Actor *Actor_GetById();
void Actor_ResetAndSetPosition();
void Animation_SetIndexAndInitializeObjects();
void FieldEvent_WaitFrames();
void Func_020001bc();
void Func_02001bcc();
void Func_02001bd4();
void Func_02001c2c();
void Func_02001c6c();
void Func_02001c7c();
void Func_02001c94();
void Func_02001ca4();
void Func_02001d04();
void Func_02001d0c();
void Camera_MoveWithinMapBounds();
void Motion_SetModeAndWaitAnimation();
void Actor_ArmCallback();
void Actor_WaitForMoveById();
void Actor_CommitPositionAndActivate();
void Actor_Jump();
void Actor_SetSpritePriority();
void Actor_SetPositionAndReset();
void Actor_SetSpeedParameters();
void Camera_FollowActor();
void Actor_WatchAndSetScript();
void Actor_SetModeById();

void Func_02000e7c(s32 id) {
  struct Actor *leader;
  s32 a;
  s32 b;
  s32 c;

  Func_02001bcc();
  Call1((void (*)())Func_02001d04, 0);
  Call1((void (*)())Func_02001c6c, (s32)MsgOh2);
  Call2((void (*)())Animation_SetIndexAndInitializeObjects, (s32)Actor_GetById(12),
        56);
  if (id == 21) {
    Call3((void (*)())Actor_SetPositionAndReset, 4, 0x178, 0x3c8);
    Call3((void (*)())Actor_ArmCallback, 4, 0, 0);
    Call4((void (*)())Func_02001d0c, 12, 16, -16, 0);
    a = 0;
    b = 0x800a;
    c = 0x5000;
  } else {
    Call3((void (*)())Actor_SetPositionAndReset, 4, 0x228, 0x3c8);
    Call3((void (*)())Actor_ArmCallback, 4, 0x8000, 0);
    Call4((void (*)())Func_02001d0c, 12, -16, -16, 0x8000);
    a = 0x8000;
    b = -10;
    c = 0x3000;
  }
  Call1((void (*)())Actor_WaitForMoveById, 12);
  Call2((void (*)())Actor_WatchAndSetScript, 4, 12);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x10000, 0x8000);
  Call2((void (*)())Camera_FollowActor, 12, 1);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 0x1d0, 0x3b8);
  Call2((void (*)())Func_02001c94, 12, a);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Value3((s32 (*)())Func_02001ca4, 12, 0x100, 30);
  Call3((void (*)())Func_02001c7c, 12, 0, 5);
  Call2((void (*)())Func_02001c94, 12, b);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Value3((s32 (*)())Func_02001ca4, 12, 0x100, 30);
  Call3((void (*)())Func_02001c7c, 12, 0, 5);
  Call3((void (*)())Actor_SetPositionAndReset, 12, 0x1d0, 0x310);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  Call3((void (*)())Actor_Jump, 12, 6, 15);
  Call3((void (*)())Actor_Jump, 12, 6, 23);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Func_02001c94, 12, c);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Func_02001c7c, 12, 0, 5);
  Call1((void (*)())FieldEvent_WaitFrames, 20);
  Call2((void (*)())Actor_SetSpritePriority, 4, 1);
  Call3((void (*)())Func_02001c2c, 4, 0x1d00000, 0x3580000);
  Call3((void (*)())Actor_SetPositionAndReset, 4, 0x1d0, 0x31c);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call2((void (*)())Motion_SetModeAndWaitAnimation, 12, 3);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 12, 2);
  leader = Actor_GetById(gAdventure.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(12, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Call1((void (*)())Actor_WaitForMoveById, 12);
  Call3((void (*)())Func_02001c2c, 12, 0, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Actor_GetById(4)->priorityFlags |= 1;
  Call3((void (*)())Actor_SetPositionAndReset, 4, 0x1d0, 0x308);
  Func_020001bc();
  Func_02001bd4();
}

void Func_0200109c(void) {
  struct Actor *leader;

  Func_02001bcc();
  Call1((void (*)())Func_02001d04, 0);
  Call1((void (*)())Func_02001c6c, (s32)MsgWhereAreYou);
  Call2((void (*)())Animation_SetIndexAndInitializeObjects, (s32)Actor_GetById(12),
        56);
  Call2((void (*)())Func_02001c94, 4, 0x4000);
  Call2((void (*)())Actor_SetSpritePriority, 12, 1);
  Call4((void (*)())Func_02001d0c, 12, 0, 16, 0xd000);
  Call1((void (*)())Actor_WaitForMoveById, 12);
  Call1((void (*)())FieldEvent_WaitFrames, 5);
  Call3((void (*)())Func_02001c7c, 12, 0, 5);
  Call3((void (*)())Actor_SetSpeedParameters, 12, 0x13333, 0x9999);
  Call2((void (*)())Actor_SetModeById, 12, 2);
  leader = Actor_GetById(gAdventure.currentOwner);
  if (leader != 0)
    Actor_ResetAndSetPosition(12, ((s16 *)&leader->x)[1],
                                     ((s16 *)&leader->z)[1]);
  Call1((void (*)())Actor_WaitForMoveById, 12);
  Call3((void (*)())Func_02001c2c, 12, 0, 0);
  Call1((void (*)())FieldEvent_WaitFrames, 10);
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, -16);
  Func_020001bc();
  Func_02001bd4();
}
