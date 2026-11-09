#include "garoh.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "event_inline.h"
void Func_020013b0(s32 id);
#define ACTOR_MODE_THEN_WAIT(id, mode, frames)                                 \
  do {                                                                         \
    Actor_SetAnimation(id, mode);                            \
    FieldEvent_Wait(frames);                                     \
  } while (0)
#define FOLLOW_LEADER(id)                                                      \
  {                                                                            \
    struct Actor *leader_;                                                     \
                                                                               \
    Actor_SetSpeed(id, 0x13333, 0x9999);          \
    Actor_SetAnimation(id, 2);                                                  \
    leader_ = Actor_Get(gMainRoutineWork.currentOwner);                    \
    if (leader_ != NULL)                                                       \
      Actor_SetDestination(id, *(s16 *)((u8 *)leader_ + 10),              \
                                *(s16 *)((u8 *)leader_ + 18));                 \
    FieldActor_WaitForMoveById(id);                                                 \
    Actor_SetPosition(id, 0, 0);                          \
  }

extern u8 MsgGaroh01[];
extern const u8 gData_0200545c[];

u32 Random16Far(void);
void Func_020052b4(void);

static inline s32 Garoh_PathStep(s32 from, s32 to, s32 i) {
  return from + ((to - from) * i) / 8;
}

void Func_02001490(void) {
  struct Actor *bird;
  s32 i;
  s32 position[3];
  s32 *pos;
  s32 x, z;
  s32 toX, toZ;

  Event_SetFlag(0x91c);
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(8, 0x600000, 0x80000);
  FieldEvent_SetMessageId((s32)MsgGaroh01);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_ShowEmote(4, 0x100, 40);
  Actor_SetSpeed(4, 0xcccc, 0x6666);
  Actor_WalkToAndWait(4, 152, 304);
  FieldActor_LaunchFromLeader(9, 32, -16, 0xb000);
  FieldActor_LaunchFromLeader(5, 0, 0, 0xc000);
  FieldActor_LaunchFromLeader(6, 16, 0, 0xb000);
  if (Event_GetFlag(7))
    FieldActor_LaunchFromLeader(7, 8, 16, 0xc000);
  Actor_WalkByAndWait(4, -16, -16);
  Actor_FaceDirection(4, 0xc000, 0);
  FieldActor_WaitForMoveById(9);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(9, 0x8000, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_ShowEmote(6, 0x101, 0);
  Actor_ShowEmote(9, 0x101, 0);
  if (Event_GetFlag(7))
    Actor_ShowEmote(7, 0x101, 0);
  Actor_ShowEmote(4, 0x101, 50);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  if (Event_GetFlag(7))
    FieldActor_StartRepeatedMotion(7, 2);
  FieldActor_StartRepeatedMotion(6, 2);
  FieldActor_StartRepeatedMotion(9, 2);
  FieldActor_StartRepeatedMotion(4, 2);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(0x900000, -1, 0x800000, 1);
  Func_020052b4();
  bird = Actor_Get(8);
  Actor_SetAnimation(8, 2);
  i = 0;
loop1:
  {
    s32 r;
    s32 wave;

    wave = Math_Sin(i << 13);
    pos = position;
    pos[0] = Garoh_PathStep(0x680000, 0x580000, i) - wave * 12;
    wave = Math_Sin(i << 13);
    position[2] = Garoh_PathStep(0x200000, 0x780000, i) - wave * 7;
    position[1] = 0;
    r = ((Random16Far() * 10) >> 16) + 4;
    Actor_SetSpeed(8, r * 0x1999, r * 0xccc);
    Actor_WalkToAndWait(8, position[0] / 0x10000, position[2] / 0x10000);
    i++;
    if (i <= 7)
      goto loop1;
  }
  Actor_SetAnimation(8, 1);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(8, 2);
  Actor_SetScript(8, gData_0200545c);
  (*(s16 *)&((struct EventWork *)Ram_HeapSlots->eventWork)->messageId)--;
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  FieldActor_Stop(8);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldCamera_FollowActor(8, 1);
  Actor_SetAnimation(8, 2);
  x = bird->x;
  z = bird->z;
  toX = 0x800000;
  toZ = 0xb80000;
  i = 0;
loop2:
  {
    s32 r;
    s32 wave;

    wave = Math_Sin(i << 13);
    position[0] = Garoh_PathStep(x, toX, i) - wave * 4;
    position[2] = Garoh_PathStep(z, toZ, i);
    position[1] = 0;
    r = ((Random16Far() * 10) >> 16) + 4;
    Actor_SetSpeed(8, r * 0x1999, r * 0xccc);
    Actor_WalkToAndWait(8, position[0] / 0x10000, position[2] / 0x10000);
    i++;
    if (i <= 7)
      goto loop2;
  }
  Actor_SetAnimation(8, 1);
  Actor_FaceDirection(8, 0x3000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, 0, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, 0x3000, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(8, 0x102, 50);
  Actor_SetSpeed(8, 0x9999, 0x4ccc);
  Actor_Get(8)->actionFlags &= 254;
  Actor_WalkToAndWait(8, 128, 148);
  FieldEvent_Wait(1);
  Actor_Get(8)->actionFlags |= 1;
  Actor_SetSpeed(8, 0x28000, 0x14000);
  Actor_WalkByAndWait(8, 0, 4);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_FaceDirection(8, 0xc000, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(8, 3);
  Actor_SetAttachedEffect(8, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(8, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_SetSpeed(8, 0x26666, 0x13333);
  Actor_WalkByAndWait(8, -24, -8);
  Actor_WalkByAndWait(8, -8, -100);
  FieldEvent_Wait(30);
  Actor_SetPosition(8, 0, 0);
  FieldCamera_MoveTo(0xa80000, -1, 0x1180000, 1);
  Func_020052b4();
  FieldEvent_Wait(10);
  Actor_ShowEmote(4, 0x102, 0);
  Actor_ShowEmote(5, 0x102, 0);
  if (Event_GetFlag(7))
    Actor_ShowEmote(7, 0x102, 0);
  Actor_ShowEmote(6, 0x102, 40);
  FieldActor_Jump(9, 6, 15);
  FieldActor_Jump(9, 6, 23);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  Actor_FaceDirection(4, 0, 0);
  if (Event_GetFlag(7))
    Actor_FaceDirection(7, 0xd000, 0);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x2006, 0, 0);
  Actor_ShowEmote(9, 0x108, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  if (Event_GetFlag(7))
    Actor_ShowEmote(7, 0x101, 0);
  Actor_ShowEmote(6, 0x101, 0);
  Actor_ShowEmote(5, 0x101, 0);
  Actor_ShowEmote(4, 0x101, 50);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x2006, 0, 0);
  Actor_ShowEmote(9, 0x100, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(6, 0x101, 50);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  if (Event_GetFlag(7))
    Actor_SetAnimation(7, 3);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(30);
  Actor_ShowEmote(9, 0x109, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 4, 33);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_WalkByAndWait(9, -8, -8);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_WalkByAndWait(9, -10, -6);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_WalkByAndWait(9, -14, -4);
  Actor_FaceDirection(4, 0xe000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  if (Event_GetFlag(7))
    Actor_FaceDirection(7, 0xc000, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(9, 0xc000, 0);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Actor_FaceDirection(5, 0xa000, 0);
  if (Event_GetFlag(7))
    Actor_FaceDirection(7, 0xb000, 0);
  FieldEvent_Wait(20);
  Event_OpenMessage(0x2005, 0);
  Actor_FaceDirection(4, 0x2000, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) == 0) {
    FieldEvent_Wait(30);
    Actor_RunRepeatedMotion(9, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(9, 0x5000, 0);
    FieldEvent_Wait(20);
    if (Event_GetFlag(7))
      Actor_FaceDirection(7, 0xc000, 0);
    FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
  } else {
    FieldEvent_Wait(40);
    Actor_RunRepeatedMotion(9, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(9, 0x5000, 0);
    FieldEvent_Wait(20);
    if (Event_GetFlag(7))
      Actor_FaceDirection(7, 0xc000, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId++;
    FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  }
  FieldActor_Jump(9, 6, 15);
  FieldActor_Jump(9, 6, 23);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, 0xc000, 0);
  FieldEvent_Wait(15);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirection(4, 0xe000, 0);
  FieldEvent_Wait(25);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(8);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(8);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x101, 30);
  FieldEvent_ShowMessageThenWait(0x2006, 0, 0);
  FieldActor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Func_020013b0(6);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2006, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(9, 0x6666, 0x3333);
  Actor_Get(9)->actionFlags &= 254;
  Actor_WalkByAndWait(9, 0, -16);
  FieldEvent_Wait(1);
  Actor_Get(9)->actionFlags |= 1;
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(9, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Actor_FaceDirection(5, 0xa000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirection(6, 0x6000, 0);
  Actor_FaceDirection(4, 0x2000, 0);
  if (Event_GetFlag(7))
    Actor_FaceDirection(7, 0xb000, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(6, 3);
  if (Event_GetFlag(7))
    Actor_SetAnimation(7, 3);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(35);
  if (Event_GetFlag(7))
    Actor_FaceDirection(7, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  Actor_FaceDirection(4, 0xe000, 0);
  FieldEvent_Wait(25);
  Actor_ShowEmote(9, 0x109, 30);
  FieldEvent_Wait(20);
  Actor_SetSpeed(9, 0x10000, 0x8000);
  Actor_WalkByAndWait(9, 0, 16);
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x103, 30);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Actor_ShowEmote(4, 0x100, 0);
  Actor_ShowEmote(5, 0x100, 0);
  if (Event_GetFlag(7))
    Actor_ShowEmote(7, 0x100, 0);
  Actor_ShowEmote(6, 0x100, 50);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Actor_SetSpeed(9, 0x20000, 0x10000);
  Actor_WalkByAndWait(9, -16, 0);
  Actor_FaceDirection(5, 0xa000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(9, 0x4000, 0);
  if (Event_GetFlag(7))
    Actor_FaceDirection(7, 0xa000, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(4, 0x102, 40);
  ACTOR_MODE_THEN_WAIT(9, 4, 33);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  FieldActor_Jump(9, 6, 15);
  FieldActor_Jump(9, 6, 23);
  if (Event_GetFlag(7))
    Actor_ShowEmote(7, 0x102, 0);
  Actor_ShowEmote(6, 0x102, 0);
  Actor_ShowEmote(4, 0x102, 0);
  Actor_ShowEmote(5, 0x102, 50);
  FieldEvent_Wait(40);
  Actor_FaceDirectionAndWait(4, 0x2000);
  FieldEvent_Wait(20);
  Actor_SetAnimation(6, 3);
  Actor_SetAnimation(4, 3);
  if (Event_GetFlag(7))
    Actor_SetAnimation(7, 3);
  ACTOR_MODE_THEN_WAIT(5, 3, 20);
  FieldEvent_Wait(30);
  FOLLOW_LEADER(9);
  FOLLOW_LEADER(5);
  FOLLOW_LEADER(6);
  if (Event_GetFlag(7)) {
    FOLLOW_LEADER(7);
  }
  FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
  Func_020051b4();
}
