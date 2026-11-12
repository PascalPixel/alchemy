/// @file
/// Scene code of overlay 0x65f, in ROM order, from push's offset up to the next object the overlay links.

#include "garoh.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "call.h"
#include "field.h"

void Func_02005304(s32 a, s32 b);
void Func_0200530c(s32 a, s32 b);
void Func_02005314(void);
void Func_0200531c(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void BlendBack(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
  Func_0802b450(a0, a1, a2, a3, a4, a5);
}

/// The fourth argument is passed unextended (0xf000), where spawn.c's
/// definition takes an s16: the callers saw an int parameter.
void Func_02001360(s32 x, s32 y, s32 z, s32 value, s32 timer);

void Func_020013b0(s32 id) {
  struct Actor *object = Actor_Get(id);

  Func_02001360(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                20);
  Event_PlaySound(151);
  FieldEvent_Wait(20);
  Func_02001360(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                20);
  Event_PlaySound(151);
  FieldEvent_Wait(20);
}

void Func_02001418(s32 actor) {
  Func_02005304(144, 1);
  Func_0200530c(actor, 0);
  Field_DispatchTypeHandler(1);
  BlendBack(44, 5, 3, 3, 19, 5);
  Func_02005314();
  Func_0200531c();
}

void Func_02001454(s32 actor) {
  Func_02005304(144, 1);
  Func_0200530c(actor, 0);
  Field_DispatchTypeHandler(3);
  BlendBack(39, 5, 3, 3, 19, 5);
  Func_02005314();
  Func_0200531c();
}

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

extern u8 MsgGaroh13[];

void Func_020022f4(void) {
  s32 v5;
  s32 t1;
  s32 t2;
  s32 t3;
  s32 b1;

  Event_SetFlag(0x91d);
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(((s32)MsgGaroh13));
  Actor_SetPosition(8, PIXELS(144), PIXELS(752));
  Actor_WalkToAndWait(4, 280, 776);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  FieldActor_LaunchFromLeader(9, 16, 0, FACING_NORTHWEST + FACING_STEP);
  FieldActor_WaitForMoveById(9);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  FieldActor_StartRepeatedMotion(4, 3);
  Actor_SetAttachedEffect(4, 0x102);
  FieldEvent_Wait(45);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(PIXELS(280), -1, PIXELS(752), 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_SetPosition(8, PIXELS(144), PIXELS(720));
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 168, 720);
  FieldEvent_Wait(10);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  FieldActor_Jump(8, 6, 0);
  Actor_WalkToAndWait(8, 200, 720);
  FieldEvent_Wait(10);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 216, 712);
  Actor_WalkToAndWait(8, 320, 712);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  Actor_SetSpeed(8, 0x9999, 0x4ccc);
  t1 = ((s32)Actor_Get(8));
  *(u8 *)((t1 + 90)) = (254 & *(u8 *)((t1 + 90)));
  Actor_WalkToAndWait(8, 296, 712);
  FieldEvent_Wait(1);
  t2 = ((s32)Actor_Get(8));
  /* FAKEMATCH(temporary): a local holding the constant 1, so the value is ORed
   * into the register that holds it; as one |= 1, 4 of 362 instructions differ
   * in tla-en. */
  b1 = 1;
  b1 |= *(u8 *)((t2 + 90));
  *(u8 *)((t2 + 90)) = b1;
  FieldEvent_Wait(15);
  Actor_FaceDirection(8, FACING_WEST + FACING_STEP, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(8, FACING_NORTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(8, FACING_WEST + FACING_STEP, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(25);
  Actor_ShowEmote(8, 0x102, 45);
  FieldActor_Jump(8, 6, 15);
  FieldActor_Jump(8, 6, 23);
  Actor_SetSpeed(8, 0x26666, 0x13333);
  Actor_WalkToAndWait(8, 200, 712);
  FieldEvent_Wait(3);
  Actor_SetSpeed(8, 0x20000, 0x10000);
  FieldActor_Jump(8, 6, 0);
  Actor_WalkToAndWait(8, 168, 712);
  FieldEvent_Wait(3);
  Actor_SetSpeed(8, 0x26666, 0x13333);
  Actor_WalkToAndWait(8, 144, 712);
  {
    s32 a2 = 0;
    /* FAKEMATCH(barrier): an empty asm on a2, so the third argument is loaded
     * first; as the plain call, 2 of 362 instructions differ in tla-en. */
    asm volatile("" : "+l"(a2));
    Actor_SetPosition(8, 0, a2);
  }
  FieldEvent_Wait(30);
  v5 = ((s32)&gMainRoutineWork);
  v5 = (v5 + 0x214);
  {
    s32 a0 = *(s32 *)(v5);
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 362 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldCamera_FollowActor(a0, 1);
  }
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x102, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(4, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldActor_Jump(9, 6, 15);
  FieldActor_Jump(9, 6, 23);
  {
    s32 a0 = 4;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 362 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    Actor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  Actor_SetAnimation(9, 2);
  t3 = ((s32)Actor_Get(*(s32 *)(v5)));
  if (t3 == 0)
    goto L_0200a66c;
  Actor_SetDestination(9, *(s16 *)(t3 + 10), *(s16 *)(t3 + 18));
L_0200a66c:;
  FieldActor_WaitForMoveById(9);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 362 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    Actor_SetPosition(a0, 0, 0);
  }
  Func_020051b4();
}

s32 Func_02001344();

s32 Func_020052bc();

extern u8 MsgGaroh14[];

void Func_02002694(void) {
  s32 v5;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t6;
  s32 t8;
  s32 t9;
  s32 t10;
  s32 t12;
  s32 t13;
  s32 t14;
  s32 t15;
  s32 t16;
  s32 t17;
  s32 t19;
  s32 t20;
  s32 t21;
  s32 t22;
  s32 t23;
  s32 t24;
  s32 b1;
  s32 p1;
  s32 p2;

  Event_SetFlag(0x91e);
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(((s32)MsgGaroh14));
  Actor_SetPosition(10, PIXELS(328), PIXELS(40));
  Actor_SetPosition(8, PIXELS(328), PIXELS(40));
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  Actor_WalkToAndWait(4, 320, 224);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x8009, 0, 0);
  FieldActor_LaunchFromLeader(9, -16, -8, FACING_NORTH);
  FieldActor_WaitForMoveById(9);
  FieldEvent_Wait(30);
  FieldCamera_SetSpeed(0x10000, 0x2000);
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(152), 1);
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(4, FACING_NORTHWEST, 0);
  {
    s32 a0 = 9;
    s32 a1 = FACING_SOUTHEAST;
    /* FAKEMATCH(barrier): an empty asm on a0 and a1, so the third argument is
     * loaded, then the first, then the second finished; as the plain call, 2 of
     * 1799 instructions differ in tla-en. */
    __asm__("" : "+l"(a0), "+l"(a1));
    Actor_FaceDirection(a0, a1, 0);
  }
  {
    s32 a0 = 4;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    Func_020052bc(a0, 1);
  }
  Func_020052b4();
  FieldCamera_SetSpeed(0x18000, 0x3000);
  Event_OpenMessage(9, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(4, 0) != 0)
    goto L_0200a818;
  FieldEvent_Wait(20);
  Actor_ShowEmote(9, 0x109, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  goto L_0200a846;
L_0200a818:;
  FieldEvent_Wait(30);
  Actor_ShowEmote(9, 0x109, 30);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
L_0200a846:;
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_RunRepeatedMotion(4, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(9, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldActor_LaunchFromLeader(5, 16, 0, FACING_NORTH);
  FieldActor_LaunchFromLeader(6, 32, 0, FACING_NORTH);
  t2 = Event_GetFlag(7);
  if (t2 == 0)
    goto L_0200a8b2;
  FieldActor_LaunchFromLeader(7, 48, -8, FACING_NORTHWEST);
L_0200a8b2:;
  FieldActor_WaitForMoveById(6);
  FieldEvent_Wait(20);
  Actor_ShowEmote(5, 0x100, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(6, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_ShowEmote(9, 0x101, 30);
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(25);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  Actor_FaceDirection(5, FACING_EAST, 0);
  FieldEvent_Wait(40);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  Actor_FaceDirection(5, FACING_NORTHWEST, 0);
  FieldEvent_Wait(25);
  Actor_SetAnimation(6, 3);
  {
    s32 a0 = 5;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(144), 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_SetPosition(11, PIXELS(328), PIXELS(136));
  t3 = Actor_Get(11);
  Actor_SetSingleChildFlags(t3, 0);
  Func_02001418(11);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_SetSpeed(8, 0xb333, 0x5999);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  Actor_SetPosition(10, PIXELS(328), PIXELS(128));
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(10, 0, 16);
  Actor_SetPosition(8, PIXELS(328), PIXELS(128));
  FieldEvent_Wait(10);
  Actor_WalkBy(8, 0, 16);
  Actor_WalkByAndWait(10, 0, 16);
  FieldEvent_Wait(10);
  Func_02001454(11);
  Actor_SetPosition(11, 0, 0);
  FieldCamera_FollowActor(10, 1);
  FieldEvent_Wait(20);
  Actor_WalkBy(8, 0, 24);
  Actor_WalkByAndWait(10, 0, 24);
  Actor_WalkByAndWait(8, 16, 8);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(PIXELS(328), -1, PIXELS(208), 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  FieldActor_StartRepeatedMotion(9, 2);
  FieldActor_StartRepeatedMotion(6, 2);
  FieldActor_StartRepeatedMotion(5, 2);
  t4 = Event_GetFlag(7);
  if (t4 == 0)
    goto L_0200ab98;
  FieldActor_StartRepeatedMotion(7, 2);
L_0200ab98:;
  Actor_RunRepeatedMotion(4, 2);
  FieldEvent_Wait(20);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(6, 0x100, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  FieldActor_StartRepeatedMotion(8, 3);
  Actor_SetAttachedEffect(8, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  FieldActor_Jump(8, 6, 15);
  FieldActor_Jump(8, 6, 23);
  Actor_FaceDirection(8, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(20);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(4, 0x101, 0);
  Actor_ShowEmote(5, 0x101, 0);
  Actor_ShowEmote(6, 0x101, 0);
  if (Event_GetFlag(7) == 0)
    goto L_0200ac86;
  Actor_ShowEmote(7, 0x101, 0);
L_0200ac86:;
  Actor_ShowEmote(9, 0x101, 60);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  Event_OpenMessage(10, 0);
  t6 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t6 == 0)
    goto L_0200ace6;
  FieldEvent_Wait(40);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldEvent_ShowMessageThenWait(a0, 0, 0);
  }
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  goto L_0200ad16;
L_0200ace6:;
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
L_0200ad16:;
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(30);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(5, 0x101, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(6, 0x106, 30);
  Actor_FaceDirection(6, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(10, 0x101, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(4, 0x101, 0);
  Actor_ShowEmote(5, 0x101, 0);
  Actor_ShowEmote(6, 0x101, 0);
  if (Event_GetFlag(7) == 0)
    goto L_0200ae32;
  Actor_ShowEmote(7, 0x101, 0);
L_0200ae32:;
  Actor_ShowEmote(9, 0x101, 60);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  Call2(FieldMessage_ShowAtActor, 10, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  t8 = Event_GetFlag(7);
  if (t8 == 0)
    goto L_0200ae8e;
  Actor_FaceDirection(7, FACING_WEST, 0);
L_0200ae8e:;
  FieldEvent_Wait(20);
  t9 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t9 == 0)
    goto L_0200aed2;
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  goto L_0200af08;
L_0200aed2:;
  FieldEvent_Wait(30);
  {
    s32 a0 = 5;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
L_0200af08:;
  Actor_FaceDirection(10, FACING_NORTHEAST, 0);
  Actor_FaceDirection(8, FACING_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  t10 = Event_GetFlag(7);
  if (t10 == 0)
    goto L_0200af6a;
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
L_0200af6a:;
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x100, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  FieldActor_SetAnimation(6, 3);
  if (Event_GetFlag(7) == 0)
    goto L_0200affc;
  FieldActor_SetAnimation(7, 3);
L_0200affc:;
  FieldActor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(8, 0x102, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(10, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirection(10, FACING_SOUTHEAST + FACING_STEP, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(6, 0x109, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(8, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_FaceDirection(9, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(30);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(5, 0x102, 30);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(8, 0, 16);
  FieldEvent_Wait(10);
  FieldActor_Jump(8, 6, 15);
  FieldActor_Jump(8, 6, 23);
  Actor_ShowEmote(6, 0x102, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_SetSpeed(10, 0x28000, 0x14000);
  Actor_SetSpeed(8, 0x28000, 0x14000);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 8, 0);
  FieldActor_Jump(8, 4, 0);
  t12 = Actor_Get(10);
  v5 = 254;
  *(u8 *)((t12 + 90)) = (v5 & *(u8 *)((t12 + 90)));
  Actor_WalkBy(10, -8, 0);
  t13 = Actor_Get(8);
  v5 = (v5 & *(u8 *)((t13 + 90)));
  *(u8 *)((t13 + 90)) = v5;
  Actor_WalkByAndWait(8, 16, 0);
  FieldEvent_Wait(1);
  t14 = Actor_Get(8);
  /* FAKEMATCH(temporary): a local holding the constant 1, so the value is ORed
   * into the register that holds it; as one |= 1, 4 of 1799 instructions differ
   * in tla-en. */
  b1 = 1;
  b1 |= *(u8 *)((t14 + 90));
  *(u8 *)((t14 + 90)) = b1;
  FieldActor_SetAnimation(8, 5);
  FieldEvent_Wait(20);
  Actor_SetSpeed(8, 0x14ccc, 0xa666);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_Wait(10);
  Actor_ShowEmote(8, 0x102, 50);
  Actor_SetAnimation(8, 1);
  FieldActor_Jump(8, 6, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_SetAttachedEffect(8, 0x102);
  {
    s32 a0 = 8;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(15);
  Actor_SetAttachedEffect(8, 0x102);
  {
    s32 a0 = 8;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(8, 0x101, 30);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(8, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(8, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(10, FACING_SOUTH + FACING_STEP, 0);
  FieldEvent_Wait(45);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  {
    s32 a0 = 8;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  FieldEvent_Wait(35);
  {
    s32 a0 = 8;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(8, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  FieldActor_WatchAndSetScript(4, 8);
  FieldActor_WatchAndSetScript(9, 8);
  FieldActor_WatchAndSetScript(5, 8);
  FieldActor_WatchAndSetScript(6, 8);
  FieldActor_WatchAndSetScript(10, 8);
  t15 = Event_GetFlag(7);
  if (t15 == 0)
    goto L_0200b46e;
  FieldActor_WatchAndSetScript(7, 8);
L_0200b46e:;
  Actor_WalkToAndWait(8, 376, 208);
  Actor_WalkByAndWait(8, 0, 16);
  Actor_WalkByAndWait(8, -16, 24);
  Actor_WalkByAndWait(8, -32, 16);
  Actor_WalkByAndWait(8, 0, 48);
  FieldEvent_Wait(20);
  FieldActor_Stop(4);
  FieldActor_Stop(9);
  FieldActor_Stop(5);
  FieldActor_Stop(6);
  FieldActor_Stop(10);
  t16 = Event_GetFlag(7);
  if (t16 == 0)
    goto L_0200b4da;
  FieldActor_Stop(7);
L_0200b4da:;
  Actor_SetPosition(8, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  Actor_FaceDirection(5, FACING_NORTH, 0);
  Actor_FaceDirection(6, FACING_NORTH, 0);
  Actor_FaceDirection(4, FACING_NORTH, 0);
  t17 = Event_GetFlag(7);
  if (t17 == 0)
    goto L_0200b542;
  Actor_FaceDirection(7, FACING_NORTHWEST, 0);
L_0200b542:;
  FieldEvent_Wait(30);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(9, 0x102, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x108, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(6, 0x101, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_ShowEmote(10, 0x109, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(9, 0x100, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(5, FACING_SOUTH, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 4);
  }
  FieldEvent_Wait(33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_ShowEmote(5, 0x101, 50);
  Actor_FaceDirection(5, FACING_NORTHWEST + FACING_STEP, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a0 = 10;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 4 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(9, FACING_NORTHEAST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  {
    s32 a0 = 9;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(5, FACING_WEST, 0);
  Actor_FaceDirection(6, FACING_WEST, 0);
  if (Event_GetFlag(7) == 0)
    goto L_0200b6f4;
  Actor_FaceDirection(7, FACING_WEST, 0);
L_0200b6f4:;
  FieldEvent_Wait(20);
  Actor_SetAnimation(5, 3);
  FieldActor_SetAnimation(6, 3);
  t19 = Event_GetFlag(7);
  if (t19 == 0)
    goto L_0200b71c;
  FieldActor_SetAnimation(7, 3);
L_0200b71c:;
  FieldActor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  {
    s32 a0 = 4;
    /* FAKEMATCH(barrier): an empty asm on a0, so the first argument is loaded
     * before the others; as the plain call, 2 of 1799 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(a0));
    FieldActor_SetAnimation(a0, 3);
  }
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  FieldActor_SetAnimation(9, 2);
  p1 = (s32)&gMainRoutineWork;
  v5 = (p1 + 0x214);
  t20 = Actor_Get(*(s32 *)(v5));
  if (t20 == 0)
    goto L_0200b77a;
  Actor_SetDestination(9, *(s16 *)(t20 + 10), *(s16 *)(t20 + 18));
L_0200b77a:;
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  FieldActor_SetAnimation(5, 2);
  t21 = Actor_Get(*(s32 *)(v5));
  if (t21 == 0)
    goto L_0200b7b8;
  Actor_SetDestination(5, *(s16 *)(t21 + 10), *(s16 *)(t21 + 18));
L_0200b7b8:;
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  FieldActor_SetAnimation(6, 2);
  t22 = Actor_Get(*(s32 *)(v5));
  if (t22 == 0)
    goto L_0200b7f6;
  Actor_SetDestination(6, *(s16 *)(t22 + 10), *(s16 *)(t22 + 18));
L_0200b7f6:;
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  t23 = Event_GetFlag(7);
  if (t23 == 0)
    goto L_0200b84e;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  FieldActor_SetAnimation(7, 2);
  t24 = Actor_Get(*(s32 *)(v5));
  if (t24 == 0)
    goto L_0200b83e;
  Actor_SetDestination(7, *(s16 *)(t24 + 10), *(s16 *)(t24 + 18));
L_0200b83e:;
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_0200b84e:;
  p2 = (s32)&gMainRoutineWork;
  FieldCamera_FollowActor(*(s32 *)((p2 + 0x214)), 1);
  Func_02001344();
  FieldEvent_SetExitRequest(11);
  Func_020051b4();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 __divsi3();
extern u8 MsgGaroh28[];

void Func_02003878(void) {
  s32 v5;
  s32 v6;
  s32 v7;
  s32 v8;
  s32 t1;
  s32 t2;
  s32 t3;
  s32 t4;
  s32 t5;
  s32 t6;
  s32 t7;
  s32 t8;
  s32 t9;
  s32 t10;
  s32 t11;
  s32 t12;
  s32 t13;
  s32 t14;
  s32 t15;
  s32 t16;
  s32 t17;
  s32 t18;
  s32 t19;
  s32 t20;
  s32 t21;
  s32 t22;
  s32 t23;
  s32 t24;
  s32 t25;
  s32 t26;
  s32 t27;
  s32 t28;
  s32 t29;
  s32 t30;
  s32 t31;
  s32 b1;
  s32 p1;
  s32 p2;

  Event_SetFlag(0x91f);
  Func_020051ac();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId(((s32)MsgGaroh28));
  Actor_SetPosition(10, 0x1480000, 0x280000);
  Actor_FaceDirection(10, 0x4000, 0);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_SetSpeed(4, 0xcccc, 0x6666);
  Actor_WalkToAndWait(4, 0x140, 224);
  FieldActor_LaunchFromLeader(9, -16, 0, 0xc000);
  FieldActor_LaunchFromLeader(5, 16, 0, 0xc000);
  FieldActor_LaunchFromLeader(6, 32, 0, 0xc000);
  t1 = Event_GetFlag(7);
  if (t1 == 0)
    goto L_0200b920;
  FieldActor_LaunchFromLeader(7, 48, 0, 0xc000);
L_0200b920:;
  Actor_FaceDirection(4, 0xc000, 0);
  FieldActor_WaitForMoveById(6);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(0x1480000, -1, 0x900000, 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  Actor_SetPosition(11, 0x1480000, 0x880000);
  t2 = (s32)Actor_Get(11);
  Actor_SetSingleChildFlags(t2, 0);
  Func_02001418(11);
  FieldEvent_Wait(30);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  Actor_SetPosition(10, 0x1480000, 0x800000);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(10, 0, 16);
  FieldEvent_Wait(20);
  Func_02001454(11);
  Actor_SetPosition(11, 0, 0);
  FieldCamera_FollowActor(10, 1);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 0, 48);
  Actor_FaceDirection(9, 0xe000, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  t3 = Event_GetFlag(7);
  if (t3 == 0)
    goto L_0200ba1e;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200ba1e:;
  FieldCamera_MoveTo(0x1480000, -1, 0xb80000, 1);
  Func_020052b4();
  FieldEvent_Wait(10);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  t4 = Event_GetFlag(7);
  if (t4 == 0)
    goto L_0200ba8c;
  Actor_SetAnimation(7, 3);
  goto L_0200ba8c;
L_0200ba8c:;
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 0, -16);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_WalkByAndWait(10, -8, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x107, 30);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_ShowEmote(9, 0x109, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_WalkByAndWait(10, 16, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  {
    s32 a1 = 0x100;
    s32 a2 = 20;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(5, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_FaceDirection(10, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_WalkByAndWait(10, -8, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(35);
  Actor_WalkByAndWait(10, -8, 0);
  {
    s32 a1 = 0x5000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(25);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_ShowEmote(9, 0x103, 45);
  FieldActor_Jump(9, 6, 15);
  FieldActor_Jump(9, 6, 23);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x108, 30);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, 0xc000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x600a, 0, 0);
  Actor_WalkByAndWait(10, 4, -12);
  FieldEvent_Wait(10);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  {
    s32 a1 = 4;
    s32 a2 = -12;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_WalkByAndWait(10, a1, a2);
  }
  FieldEvent_Wait(10);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_FaceDirection(10, 0x4000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x105, 30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  {
    s32 a1 = 0x101;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(6, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  {
    s32 a1 = 0x3000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  {
    s32 a1 = 0x102;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(10, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a1 = 0x5000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_FaceDirection(9, 0x2000, 0);
  Actor_FaceDirection(4, 0xa000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(5, 0, 0);
  t5 = Event_GetFlag(7);
  if (t5 == 0)
    goto L_0200bf32;
  Actor_FaceDirection(7, 0x8000, 0);
L_0200bf32:;
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  t6 = Event_GetFlag(7);
  if (t6 == 0)
    goto L_0200bf6a;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200bf6a:;
  Actor_FaceDirection(9, 0xe000, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  FieldEvent_Wait(25);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x101, 30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  {
    s32 a1 = 0x100;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(10, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  {
    s32 a1 = 0x2000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(9, a1, a2);
  }
  FieldEvent_Wait(30);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(15);
  {
    s32 a1 = 0xc000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(30);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_SetAnimation(4, 3);
  ACTOR_MODE_THEN_WAIT(5, 3, 20);
  FieldEvent_Wait(30);
  {
    s32 a1 = 0xe000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(9, a1, a2);
  }
  FieldEvent_Wait(25);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x100, 30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_FaceDirection(9, 0, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_WalkByAndWait(9, 16, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 16, 0);
  Actor_FaceDirection(4, 0xe000, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 16, 0);
  Actor_FaceDirection(10, 0x3000, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  t7 = Event_GetFlag(7);
  if (t7 == 0)
    goto L_0200c216;
  Actor_FaceDirection(7, 0xc000, 0);
L_0200c216:;
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 16, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveTo(0x1580000, -1, 0xd00000, 1);
  Func_020052b4();
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_SetSpeed(10, 0x13333, 0x9999);
  Actor_WalkByAndWait(10, 16, 16);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  t8 = Event_GetFlag(7);
  if (t8 == 0)
    goto L_0200c2bc;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200c2bc:;
  Actor_FaceDirection(4, 0xe000, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  Actor_FaceDirection(9, 0xa000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_RunRepeatedMotion(5, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(5, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 4, 33);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(10, 0x2000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x106, 40);
  Actor_FaceDirection(6, 0x8000, 0);
  FieldEvent_Wait(20);
  Event_OpenMessage(6, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_FaceDirection(4, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  t9 = Event_GetFlag(7);
  if (t9 == 0)
    goto L_0200c400;
  Actor_FaceDirection(7, 0x8000, 0);
L_0200c400:;
  t10 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t10 != 0)
    goto L_0200c434;
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  goto L_0200c456;
L_0200c434:;
  FieldEvent_Wait(40);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
L_0200c456:;
  {
    s32 a1 = 0x101;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  Actor_ShowEmote(5, 0x101, 0);
  t11 = Event_GetFlag(7);
  if (t11 == 0)
    goto L_0200c484;
  Actor_ShowEmote(7, 0x101, 0);
L_0200c484:;
  {
    s32 a1 = 0x101;
    s32 a2 = 50;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(10, a1, a2);
  }
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_FaceDirection(6, 0xe000, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_ShowEmote(9, 0x101, 30);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(6, 0x107, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_FaceDirection(6, 0xc000, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(6, 0x103, 30);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  ACTOR_MODE_THEN_WAIT(6, 4, 33);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  {
    s32 a1 = 0x105;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(9, a1, a2);
  }
  Actor_ShowEmote(10, 0x105, 50);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(10, 3);
  t12 = Event_GetFlag(7);
  if (t12 == 0)
    goto L_0200c596;
  Actor_SetAnimation(7, 3);
L_0200c596:;
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(6, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldActor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(4, 0xe000, 0);
  Actor_FaceDirection(5, 0xe000, 0);
  Actor_FaceDirection(9, 0xa000, 0);
  Actor_FaceDirection(10, 0x2000, 0);
  FieldEvent_Wait(25);
  FieldActor_Jump(9, 6, 15);
  FieldActor_Jump(9, 6, 23);
  FieldEvent_Wait(10);
  Actor_SetAnimation(9, 3);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  v5 = 0;
  FieldEvent_ShowTwoMessagesAndWait(10, 9, 2, 9, 5, 9, 17, 14, 20, 9, v5);
  t13 = Event_GetFlag(7);
  if (t13 == 0)
    goto L_0200c694;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200c694:;
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  FieldActor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, 0x102);
  FieldEvent_Wait(40);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_ShowEmote(10, 0x100, 30);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  ACTOR_MODE_THEN_WAIT(6, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(9, 0x6000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(9, 0xa000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  Actor_WalkByAndWait(9, -18, 0);
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Actor_FaceDirection(9, 0xc000, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(5, 3, 20);
  FieldEvent_Wait(10);
  t14 = Event_GetFlag(7);
  if (t14 == 0)
    goto L_0200c880;
  Actor_FaceDirection(9, 0x2000, 0);
  FieldEvent_ShowMessageThenWait(0x2009, 0, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(20);
  Actor_SetAnimation(7, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(10);
  goto L_0200c892;
L_0200c880:;
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
L_0200c892:;
  Actor_WalkByAndWait(9, -42, 0);
  Actor_FaceDirection(4, 0xc000, 0);
  {
    s32 a1 = 0x3000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(9, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(9, 0xe000, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, -16, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  {
    s32 a1 = 0x5000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(30);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(25);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 4, 33);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(10, 0xc000, 0);
  FieldEvent_Wait(40);
  Func_020013b0(10);
  Func_020013b0(10);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 16, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldEvent_Wait(40);
  Actor_FaceDirection(10, 0x8000, 0);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, -16, 0);
  FieldEvent_Wait(10);
  {
    s32 a1 = -16;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    Actor_WalkByAndWait(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  {
    s32 a0 = 10;
    Actor_FaceDirection(a0, 0, 0);
  }
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 16, 0);
  FieldEvent_Wait(20);
  Actor_FaceDirection(10, 0xc000, 0);
  FieldEvent_Wait(20);
  FieldActor_StartRepeatedMotion(10, 3);
  Actor_SetAttachedEffect(10, 0x102);
  FieldEvent_Wait(40);
  Actor_WalkByAndWait(10, 0, -12);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(10, 0, -12);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, 0x106, 40);
  Actor_FaceDirection(10, 0x4000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(10, 0, 0);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(5);
  t15 = (s32)Actor_Get(12);
  v5 = t15;
  v8 = *(s32 *)(v5 + 80);
  *(s32 *)(v8 + 12) = 0x6666;
  FieldActor_SetSpritePriority(12, 1);
  t16 = (s32)Actor_Get(12);
  Actor_SetSingleChildFlags(t16, 0);
  Actor_SetPosition(12, 0x1480000, 0xa00000);
  FieldActor_Jump(12, 9, 0);
  v7 = 0;
  v6 = 20;
L_0200caf6:;
  t17 = Math_Divide(v7, 20);
  *(s32 *)(v8 + 12) = (t17 + 0x6666);
  {
    s32 angle = *(u16 *)(v5 + 6);
    /* FAKEMATCH(barrier): the native starts the 0x1000 step before it loads
     * the angle; as the plain sum, 2 of 2484 instructions differ in tla-en. */
    __asm__("" : "+l"(angle));
    *(u16 *)(v5 + 6) = angle + 0x1000;
  }
  System_Wait(1);
  v6 = (v6 - 1);
  v7 = (v7 + 0x9999);
  if (v6 >= 0)
    goto L_0200caf6;
  FIELD_AT(v5, u16, 6) = 0x4000;
  {
    s32 a2 = 8;
    s32 a1 = 0;

    Actor_WalkByAndWait(12, a1, a2);
  }
  t18 = (s32)Actor_Get(12);
  Actor_SetSingleChildFlags(t18, 1);
  FieldEvent_Wait(5);
  Actor_WalkByAndWait(12, 0, 8);
  FieldEvent_Wait(5);
  Actor_WalkByAndWait(12, 0, 8);
  FieldEvent_Wait(5);
  Actor_WalkByAndWait(12, 0, 8);
  FieldEvent_Wait(5);
  Actor_ShowEmote(5, 0x100, 30);
  FieldEvent_ShowMessageThenWait(0x2005, 0, 0);
  Actor_ShowEmote(10, 0x101, 30);
  Actor_FaceDirection(10, 0x3000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_WalkByAndWait(10, 0, 24);
  Actor_FaceDirection(10, 0x3000, 0);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  FieldEvent_Wait(20);
  {
    s32 a1 = 0;

    Event_OpenMessage(0x200a, a1);
  }
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  t19 = Event_GetFlag(7);
  if (t19 == 0)
    goto L_0200cc42;
  Actor_FaceDirection(7, 0x8000, 0);
L_0200cc42:;
  FieldEvent_Wait(20);
  t20 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (t20 != 0)
    goto L_0200cc7c;
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  goto L_0200cca2;
L_0200cc7c:;
  FieldEvent_Wait(40);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  {
    s32 a0 = 0x200a;
    FieldEvent_ShowMessageThenWait(a0, 0, 0);
  }
L_0200cca2:;
  Actor_FaceDirection(5, 0xc000, 0);
  Actor_FaceDirection(9, 0xe000, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  t21 = Event_GetFlag(7);
  if (t21 == 0)
    goto L_0200ccdc;
  Actor_FaceDirection(7, 0xa000, 0);
L_0200ccdc:;
  FieldEvent_Wait(30);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  Actor_SetSpeed(10, 0x26666, 0x13333);
  t22 = (s32)Actor_Get(10);
  *(u8 *)((t22 + 90)) = (254 & *(u8 *)((t22 + 90)));
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_WalkByAndWait(10, 0, 8);
  Actor_WalkByAndWait(10, 0, -8);
  FieldEvent_Wait(1);
  t23 = (s32)Actor_Get(10);
  /* FAKEMATCH(temporary): the value is ORed into the register that holds the
   * constant; as one |=, 4 of 2484 instructions differ in tla-en. */
  b1 = 1;
  b1 |= *(u8 *)((t23 + 90));
  *(u8 *)((t23 + 90)) = b1;
  FieldEvent_Wait(1);
  Actor_SetSpeed(10, 0xb333, 0x5999);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_FaceDirection(9, 0x3000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  t24 = Event_GetFlag(7);
  if (t24 == 0)
    goto L_0200cda2;
  Actor_FaceDirection(7, 0x8000, 0);
L_0200cda2:;
  Actor_FaceDirection(6, 0x8000, 0);
  Djinn_JoinParty(12, 3, 9);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Actor_SetAnimation(4, 1);
  FieldEvent_Wait(20);
  Actor_FaceDirection(4, 0xc000, 0);
  Actor_FaceDirection(10, 0x4000, 0);
  FieldEvent_Wait(20);
  *(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) =
      (*(u16 *)(((s32)Ram_HeapSlots->eventWork + 0x1c4)) + 1);
  {
    s32 a1 = 0x108;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_ShowEmote(6, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(6, 0, 0);
  Actor_FaceDirection(5, 0, 0);
  FieldEvent_Wait(30);
  ACTOR_MODE_THEN_WAIT(5, 3, 20);
  FieldEvent_Wait(20);
  Actor_FaceDirection(9, 0xe000, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  Actor_FaceDirection(6, 0xa000, 0);
  {
    s32 a1 = 0xc000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    asm volatile("" : "+l"(a1), "+l"(a2));
    Actor_FaceDirection(5, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 4, 33);
  FieldEvent_ShowMessageThenWait(0x200a, 0, 0);
  ACTOR_MODE_THEN_WAIT(9, 3, 20);
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(10);
  {
    s32 a1 = 0x108;
    s32 a2 = 30;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    Actor_ShowEmote(5, a1, a2);
  }
  FieldEvent_ShowMessageThenWait(0x2005, 0, 0);
  {
    s32 a1 = 0x3000;
    s32 a2 = 0;
    /* FAKEMATCH(barrier): the native loads the later arguments before the
     * first; as plain calls, 177 of 2484 instructions differ in tla-en. */
    Actor_FaceDirection(10, a1, a2);
  }
  FieldEvent_Wait(20);
  ACTOR_MODE_THEN_WAIT(10, 3, 20);
  FieldEvent_Wait(20);
  Actor_SetAnimation(4, 3);
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  t25 = Event_GetFlag(7);
  if (t25 == 0)
    goto L_0200cf2a;
  Actor_SetAnimation(7, 3);
L_0200cf2a:;
  Actor_SetAnimation(9, 3);
  FieldEvent_Wait(20);
  FieldEvent_Wait(20);
  Actor_WalkByAndWait(9, 12, 0);
  Actor_FaceDirection(10, 0x5000, 0);
  Actor_FaceDirection(6, 0x8000, 0);
  Actor_FaceDirection(5, 0x8000, 0);
  Actor_FaceDirection(9, 0x4000, 0);
  FieldEvent_Wait(20);
  Actor_RunRepeatedMotion(9, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(9, 0, 0);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(20);
  t26 = Event_GetFlag(7);
  if (t26 == 0)
    goto L_0200cfbc;
  Actor_SetAnimation(7, 3);
L_0200cfbc:;
  Actor_SetAnimation(5, 3);
  Actor_SetAnimation(6, 3);
  ACTOR_MODE_THEN_WAIT(4, 3, 20);
  FieldEvent_Wait(20);
  Actor_SetSpeed(9, 0x13333, 0x9999);
  Actor_SetAnimation(9, 2);
  p1 = (s32)&gMainRoutineWork;
  v5 = (p1 + 0x214);
  t27 = (s32)Actor_Get(*(s32 *)(v5));
  if (t27 == 0)
    goto L_0200d016;
  Actor_SetDestination(9, *(s16 *)(t27 + 10), *(s16 *)(t27 + 18));
L_0200d016:;
  FieldActor_WaitForMoveById(9);
  Actor_SetPosition(9, 0, 0);
  Actor_SetSpeed(5, 0x13333, 0x9999);
  Actor_SetAnimation(5, 2);
  t28 = (s32)Actor_Get(*(s32 *)(v5));
  if (t28 == 0)
    goto L_0200d054;
  Actor_SetDestination(5, *(s16 *)(t28 + 10), *(s16 *)(t28 + 18));
L_0200d054:;
  FieldActor_WaitForMoveById(5);
  Actor_SetPosition(5, 0, 0);
  Actor_SetSpeed(6, 0x13333, 0x9999);
  Actor_SetAnimation(6, 2);
  t29 = (s32)Actor_Get(*(s32 *)(v5));
  if (t29 == 0)
    goto L_0200d092;
  Actor_SetDestination(6, *(s16 *)(t29 + 10), *(s16 *)(t29 + 18));
L_0200d092:;
  FieldActor_WaitForMoveById(6);
  Actor_SetPosition(6, 0, 0);
  t30 = Event_GetFlag(7);
  if (t30 == 0)
    goto L_0200d0fa;
  Actor_SetSpeed(7, 0x13333, 0x9999);
  Actor_SetAnimation(7, 2);
  t31 = (s32)Actor_Get(*(s32 *)(v5));
  if (t31 == 0)
    goto L_0200d0ea;
  goto L_0200d0dc;
L_0200d0dc:;
  Actor_SetDestination(7, *(s16 *)(t31 + 10), *(s16 *)(t31 + 18));
L_0200d0ea:;
  FieldActor_WaitForMoveById(7);
  Actor_SetPosition(7, 0, 0);
L_0200d0fa:;
  p2 = (s32)&gMainRoutineWork;
  {
    s32 a0 = *(s32 *)((p2 + 0x214));
    FieldCamera_FollowActor(a0, 1);
  }
  Func_020051b4();
}
