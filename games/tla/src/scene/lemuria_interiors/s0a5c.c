#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"
#include "types.h"
#include "event_inline.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_0200282c();
s32 Func_02002834();

s32 Func_02000a5c(s32 a0, s32 a1, s32 a2) {
  struct Actor *actor;
  s32 startBuf[3], targetBuf[3], posBuf[3];
  s32 *start, *target, *pos;
  s32 radius, angle;
  s32 result, i, phase, sine, answer;

  actor = (struct Actor *)((s32)Actor_GetById(11));
  result = GameFlag_Test(0x965);
  if (result != 0)
    goto L_02008a86;
  goto L_0200927c;
L_02008a86:
  Event_SetFlag(0x966);
  {
    struct Actor *other = (struct Actor *)((s32)Actor_GetById(10));
    other->priorityFlags |= 1;
  }
  Func_0200282c();
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
    actor->angle = angle + 16384;
    if (i == 128) {
      Actor_FaceDirection(10, 0x3000, 0);
    }
    System_Wait(1);
  }
  actor->angle = 0x8000;
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
    actor->angle = angle + 16384;
    if (i == 153) {
      Actor_FaceDirection(10, 0x7000, 0);
    }
    System_Wait(1);
  }
  Actor_FaceDirection(10, 0xb000, 0);
  actor->angle = 0xd000;
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
  result = Func_02002834();
L_0200927c:
  return result;
}

#include "field_event.h"

extern const u8 gData_02002dec[];

void Func_020027f4(struct FieldActor *object, s32 mode);
void Func_020027fc(struct FieldActor *object, const void *script);

struct SpritePictures {
  u8 unk00[5];
  u8 picture;
};

struct AnimatedSprite {
  u8 unk00[0x12];
  s16 rotation;
  u8 unk14[5];
  u8 picture;
  u8 nextPicture;
  u8 unk1b[0xd];
  struct SpritePictures *pictures;
};

struct SpawnedActor {
  u8 unk00[0x5e];
  u16 timer;
};

void Func_0200128c(s32 x, s32 y, s32 z, s16 rotation, s32 timer) {
  struct FieldActor *object;
  struct AnimatedSprite *sprite;

  object = Import_ActorCreate(0x1e8, x, y, z);
  if (object != 0) {
    sprite = (struct AnimatedSprite *)object->sprite;
    Func_020027f4(object, 1);
    Func_020027fc(object, gData_02002dec);
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
