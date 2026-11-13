/// @file
/// Scene code of overlay 0x65f, in ROM order, from s12a8's offset up to the next object the overlay links.

// ---- games/tla/src/scene/garoh/s12a8.c ----

#include "types.h"
#include "field_effect.h"
#include "actor.h"
#include "event_inline.h"
#include "garoh.h"
#include "field_sprite.h"

struct Vec {
  s32 x;
  s32 y;
  s32 z;
};

s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_020012a8(s32 objectId) {
  struct Actor *object;
  struct Vec vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  s32 z;

  object = Actor_Get(objectId);
  params.update = Effect_AdvanceMotion;
  // A goto loop: the vector's address is formed after the cosine call on every
  // pass, where a for loop hoists it.
  angleStep = 0;
burst:
  angle = angleStep << 12;
  vec.x = Math_Cosine(angle);
  vec.y = 0;
  z = Math_Sin(angle);
  x = vec.x;
  vec.z = z;
  x += x / 3;
  vec.x = x;
  Func_020000b8(object->x, object->y, object->z, x, vec.y, z, 0x01000001,
                &params);
  angleStep += 2;
  if (angleStep <= 16)
    goto burst;
}

struct SpawnObject {
  u8 unk00[0x50];
  struct FieldSprite *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[8];
  u16 unk5e;
};

/* VIEW(s12a8): variable gData_02006330 as extern u8 gData_02006330[]; the definition has u32 gData_02006330[] = { 38, }; tried as the definition's extern u32 gData_02006330[], it does not compile in tla-ja: games/tla/src/scene/garoh/s12a8.c:95: `gData_02006330__s12a8' undeclared (first use in this function). */
extern u8 gData_02006330__s12a8[] __asm__("gData_02006330");

void Func_02005164(struct SpawnObject *object, s32 mode);
void Actor_StartScript(struct SpawnObject *object, const void *script);

void Func_0200132c(void) {
  s32 b = 10;
  s32 c = 11;

  /* FAKEMATCH(barrier): an empty asm on b and c, so the first argument is built
   * last, after the second and third; without it, 2 of 10 instructions differ
   * in tla-en. */
  __asm__ volatile("" : "+r"(b), "+r"(c));
  Func_02005354(1, b, c);
  Flag_Set(0x203);
}

s32 Func_02001344(void) {
  if (!Flag_IsSet(0x91b))
    return Flag_IsSet(0x91e);
}

/// Twin of DriftScene_SpawnObject: store the fifth argument at 0x5e.
void Func_02001360(s32 x, s32 y, s32 z, s16 value, s32 timer) {
  struct SpawnObject *object;
  struct FieldSprite *sprite;

  object = (struct SpawnObject *)FieldActor_Create(0x1e8, x, y, z);
  if (object != 0) {
    sprite = object->sprite;
    Func_02005164(object, 1);
    Actor_StartScript(object, gData_02006330__s12a8);
    object->motionFlags = 0;
    /* FAKEMATCH(barrier): an empty asm, so the stack argument is loaded before
     * the pooled zero for nextFrame; without it, 2 of 32 instructions differ in
     * tla-en. */
    __asm__ volatile("");
    object->unk5e = timer;
    sprite->nextFrame = 0;
    sprite->rotation = value;
  }
}

// ---- games/tla/src/scene/garoh/push.c ----
// Scene code of overlay 0x65f, in ROM order, from push's offset up to the next object the overlay links, and the overlay's data.

#include "system.h"
#include "main_routine_work.h"
#include "event_work.h"
#include "scene_tables.h"
#include "ram_buffer.h"
#include "call.h"
#include "field.h"
#include "text/msg_ids.h"

void BattleEffect_LoadActionEffectResources(s32 a, s32 b);
void BattleEffect_SetupObjectPair(s32 a, s32 b);
void BattleEffect_StopMainActor(void);
void BattleEffect_CleanupSceneObjects(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void BlendBack(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
  Func_0802b450(a0, a1, a2, a3, a4, a5);
}

/// The fourth argument is passed unextended (0xf000), where spawn.c's
/// definition takes an s16: the callers saw an int parameter.
/* VIEW(push): routine Func_02001360 as void Func_02001360(s32 x, s32 y, s32 z, s32 value, s32 timer); the definition has void Func_02001360(s32 x, s32 y, s32 z, s16 value, s32 timer); tried as the definition's void Func_02001360(s32 x, s32 y, s32 z, s16 value, s32 timer), which the object already declares, Func_020013b0 moves: 4 of 43 instructions in tla-ja. */
extern void Func_02001360__push(s32 x, s32 y, s32 z, s32 value, s32 timer) __asm__("Func_02001360");

void Func_020013b0(s32 id) {
  struct Actor *object = Actor_Get(id);

  Func_02001360__push(object->x + 0x80000, object->y + 0x1c0000, object->z, 0x1000,
                20);
  Event_PlaySound(151);
  FieldEvent_Wait(20);
  Func_02001360__push(object->x - 0x80000, object->y + 0x1c0000, object->z, 0xf000,
                20);
  Event_PlaySound(151);
  FieldEvent_Wait(20);
}

void Func_02001418(s32 actor) {
  BattleEffect_LoadActionEffectResources(144, 1);
  BattleEffect_SetupObjectPair(actor, 0);
  Field_DispatchTypeHandler(1);
  BlendBack(44, 5, 3, 3, 19, 5);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
}

void Func_02001454(s32 actor) {
  BattleEffect_LoadActionEffectResources(144, 1);
  BattleEffect_SetupObjectPair(actor, 0);
  Field_DispatchTypeHandler(3);
  BlendBack(39, 5, 3, 3, 19, 5);
  BattleEffect_StopMainActor();
  BattleEffect_CleanupSceneObjects();
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

extern u32 gData_0200545c[];

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
  Event_Start();
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
  Event_End();
}

void Func_020022f4(void) {
  s32 v5;
  s32 t1;
  s32 t2;
  s32 t3;
  s32 b1;

  Event_SetFlag(0x91d);
  Event_Start();
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
  Event_End();
}

s32 Func_02001344();

s32 Func_020052bc();

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
  Event_Start();
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
  Event_End();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

s32 __divsi3();

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
  Event_Start();
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
  Event_End();
}

#include "objcmd.h"

void Func_02000420(void);
void Func_020004cc(void);
void Func_02000580(void);
void Func_020005cc(void);
void Func_02000618(void);
void Func_02000664(void);
void Func_02000698(void);
void Func_020006ec(void);
void Func_02000808(void);
void Func_02000960(void);
void Func_020009a4(void);
void Func_02000e58(void);
void Func_02000e68(void);
void Func_02000f74(void);
void Func_02000f80(void);
void Func_02000f8c(void);
void Func_02000f98(void);
void Func_02000fa4(void);
void Func_02000fbc(void);
void Func_02000fec(void);
void Func_0200101c(void);
void Func_0200105c(void);
void Func_0200108c(void);
void Func_02001098(void);
void Func_020010d4(void);
void Func_02001104(void);
void Func_02001144(void);
void Func_020011c8(void);

extern u32 gData_0200539c[];
extern u32 gData_020053d8[];
extern u32 gData_02005414[];
extern s32 gData_0200552c[];
extern s16 gData_02005d9c[];
extern s16 gData_02005db2[];
extern s16 gData_02005dc8[];

// clang-format off
u32 gData_0200539c[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020053d8[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005414[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

s16 gData_02005450[] = {
    8, 9, 10, 11, -1, 0,
};

u32 gData_0200545c[] = {
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(12),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(7),
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(34),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 gData_020054b8[] = {
    31, 31, 31, 29, 29, 30, 27, 28,
    29, 25, 27, 28, 23, 25, 27, 21,
    24, 26, 15, 18, 19, 10, 12, 13,
    5, 6, 6,
};

u16 gData_020054ee[] = {
    24, 28, 30, 19, 24, 27, 15, 20,
    25, 11, 16, 23, 7, 12, 21, 3,
    8, 19, 27, 27, 27, 16, 24, 29,
    0,
};

s32 gData_02005520[] = {
    (s32)gData_0200539c,
    (s32)gData_020053d8,
    (s32)gData_02005414,
};

s32 gData_0200552c[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, PIXELS(-4)),
    SC_ADD(SW_X, PIXELS(6)),
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_CALL(Func_02000420),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(59),
    SCENE_EXIT_IF(1, 64, 1, -1),
    SCENE_EXIT_IF(2, 65, 4, -1),
    SCENE_EXIT_IF(3, 64, 3, -1),
    SCENE_EXIT_IF(4, 64, 4, -1),
    SCENE_EXIT_IF(5, 64, 5, -1),
    SCENE_EXIT_IF(6, 66, 2, -1),
    SCENE_EXIT_IF(7, 62, 4, -1),
    SCENE_EXIT_IF(8, 62, 1, -1),
    SCENE_EXIT_IF(9, 62, 11, -1),
    SCENE_EXIT_IF(10, 60, 2, -1),
    SCENE_EXIT_IF(11, 60, 3, -1),
    SCENE_EXIT_IF(12, 63, 1, -1),
    SCENE_EXITS(60),
    SCENE_EXIT_IF(1, 61, 2, -1),
    SCENE_EXIT_IF(2, 59, 10, -1),
    SCENE_EXIT_IF(3, 59, 11, -1),
    SCENE_EXITS(61),
    SCENE_EXIT_IF(1, 2, 15, -1),
    SCENE_EXIT_IF(2, 60, 1, -1),
    SCENE_EXITS(62),
    SCENE_EXIT_IF(1, 59, 8, -1),
    SCENE_EXIT_IF(2, 62, 3, -1),
    SCENE_EXIT_IF(3, 62, 2, -1),
    SCENE_EXIT_IF(4, 59, 7, -1),
    SCENE_EXIT_IF(5, 65, 3, -1),
    SCENE_EXIT_IF(6, 65, 5, -1),
    SCENE_EXIT_IF(7, 64, 6, -1),
    SCENE_EXIT_IF(8, 64, 7, -1),
    SCENE_EXIT_IF(9, 62, 10, -1),
    SCENE_EXIT_IF(10, 62, 9, -1),
    SCENE_EXIT_IF(11, 59, 9, -1),
    SCENE_EXITS(63),
    SCENE_EXIT_IF(1, 59, 12, -1),
    SCENE_EXIT_IF(2, 63, 4, -1),
    SCENE_EXIT_IF(3, 63, 5, -1),
    SCENE_EXIT_IF(4, 63, 2, -1),
    SCENE_EXIT_IF(5, 63, 3, -1),
    SCENE_EXIT_IF(6, 63, 7, -1),
    SCENE_EXIT_IF(7, 63, 6, -1),
    SCENE_EXIT_IF(11, 64, 99, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020056dc[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020056f4[] = {
    {131, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(400), FACING_EAST, 1, 0},
    {131, CONDITION_ALWAYS, 1, PIXELS(416), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {131, CONDITION_ALWAYS, 1, PIXELS(192), 0, PIXELS(304), FACING_EAST, 1, 0},
    {131, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(344), FACING_WEST, 1, 0},
    {131, CONDITION_ALWAYS, 1, PIXELS(368), 0, PIXELS(312), FACING_SOUTH, 1, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(232), FACING_NORTH, 2, 1},
    {304, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(312), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(280), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(296), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(408), FACING_NORTH, 2, 1},
    {42, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

s16 gData_02005874[] = {
    192, -1, 1, 0, 0, 232, 0, 0,
    0, 400, 0, 1, 190, -1, 1, 0,
    0, 416, 0, 0, 0, 424, 0x4000, 1,
    185, -1, 1, 0, 0, 192, 0, 0,
    0, 304, 0, 1, 196, -1, 1, 0,
    0, 280, 0, 0, 0, 344, 0x8000, 1,
    188, -1, 1, 0, 0, 368, 0, 0,
    0, 312, 0x4000, 1, 191, -1, 2, 0,
    0, 384, 0, 0, 0, 232, 0x4000, 1,
    304, -1, 1, 0, 0, 312, 0, 0,
    0, 232, 0xc000, 2, 304, -1, 1, 0,
    0, 408, 0, 0, 0, 232, 0xc000, 258,
    304, -1, 1, 0, 0, 184, 0, 0,
    0, 232, 0xc000, 2, 304, -1, 1, 0,
    0, 88, 0, 0, 0, 312, 0xc000, 2,
    304, -1, 1, 0, 0, 152, 0, 0,
    0, 280, 0xc000, 2, 304, -1, 1, 0,
    0, 264, 0, 0, 0, 248, 0xc000, 2,
    304, -1, 1, 0, 0, 408, 0, 0,
    0, 296, 0xc000, 2, 304, -1, 1, 0,
    0, 296, 0, 0, 0, 408, 0xc000, 258,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

struct ActorPlacement gData_020059dc[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(472), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(456), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(392), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(392), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005a54[] = {
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(128), 0, PIXELS(168), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(208), 0, PIXELS(56), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(152), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(248), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(352), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(216), 0, PIXELS(424), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(96), 0, PIXELS(488), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(328), 0, PIXELS(440), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(456), 0, PIXELS(352), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(136), 0, PIXELS(664), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005b5c[] = {
    {22, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(120), FACING_NORTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005c1c[] = {
    {22, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {42, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {246, 0x75, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(64), 0, PIXELS(562), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(370), 0, PIXELS(56), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(256), 0, PIXELS(80), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(184), 0, PIXELS(352), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(392), 0, PIXELS(368), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(280), 0, PIXELS(626), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(336), 0, PIXELS(784), FACING_NORTH, 2, 1},
    {SCENE_TABLE_END},
};

s16 gData_02005d9c[] = {
    82, 32, 1, 2, 6, 80, 32, 1,
    2, 6, -1,
};

s16 gData_02005db2[] = {
    82, 35, 1, 2, 6, 80, 35, 1,
    2, 6, -1,
};

s16 gData_02005dc8[] = {
    78, 32, 2, 2, 6, 78, 35, 2,
    2, 6, -1, 0,
};

s32 gData_02005de0[] = {
    (s32)gData_02005d9c,
    0x15004e,
    (s32)gData_02005d9c,
    0,
    (s32)gData_02005d9c,
    0xa0048,
    (s32)gData_02005db2,
    0x16004a,
    (s32)gData_02005d9c,
    0x170055,
    (s32)gData_02005dc8,
    0xd004e,
};

struct Trigger gData_02005e10[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02005e34[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc401, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0xc401, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0xc402, 12, CONDITION_ALWAYS, EVENT(Func_02000960)},
    {TRIGGER_STEP, 20, 0x91e, EVENT(Func_02000808)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Func_02000580)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGaroh03)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgGaroh05)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Func_020005cc)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgGaroh07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgGaroh08)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgGaroh09)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgGaroh10)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgGaroh11)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_02000fbc)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_0200105c)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaroh31))},
    {TRIGGERS_END},
};

struct Trigger gData_02005f78[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc401, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0xc401, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0xc402, 12, CONDITION_ALWAYS, EVENT(Func_02000960)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgGaroh15)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgGaroh16)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(Func_02000664)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(Func_02000618)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgGaroh19)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(Func_020006ec)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgGaroh22)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgGaroh23)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgGaroh24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgGaroh25)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgGaroh26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1037, 0x301, EVENT(Func_020006ec)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgGaroh27)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_02000fbc)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_0200105c)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaroh31))},
    {TRIGGERS_END},
};

struct Trigger gData_020060d4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_STEP, 30, 0x91c, EVENT(Func_02001490)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_0200108c)},
    {TRIGGERS_END},
};

s32 gData_0200611c[] = {
    1,          (s32)0xffff0001, 1,
    1,          (s32)0xffff0002, 2,
    2,          0x2100014,       (s32)Func_02001144,
    2,          0x2110015,       (s32)Func_020011c8,
    0x10008c15, 0x2010008,       (s32)Func_02000e58,
    0x8c15,     0x2010008,       (s32)Func_02000f74,
    0x10008c15, 0x2020009,       (s32)Func_02000e58,
    0x8c15,     0x2020009,       (s32)Func_02000f80,
    0x10008c15, 0x203000a,       (s32)Func_02000e58,
    0x8c15,     0x203000a,       (s32)Func_02000f8c,
    0x10008c15, 0x204000b,       (s32)Func_02000e58,
    0x8c15,     0x204000b,       (s32)Func_02000f98,
    8,          0x2010000,       (s32)Func_02000e68,
    9,          0x2010000,       (s32)Func_02000fa4,
    0x40009085, -0x10000,        (s32)Func_02001098,
};

struct Trigger gData_020061d0[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0x21, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {0x21, 8, CONDITION_ALWAYS, 8},
    {0x21, 9, CONDITION_ALWAYS, 9},
    {0x31, 10, CONDITION_ALWAYS, 10},
    {0x21, 11, CONDITION_ALWAYS, 11},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_02000fec)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020010d4)},
    {TRIGGERS_END},
};

struct Trigger gData_02006278[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {0xc402, 6, CONDITION_ALWAYS, EVENT(Func_02000960)},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {0x21, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 30, 0x91d, EVENT(Func_020022f4)},
    {TRIGGER_STEP, 31, CONDITION_ALWAYS, EVENT(Func_020009a4)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgGaroh29)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgGaroh30)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_0200101c)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_02001104)},
    {TRIGGERS_END},
};

u32 gData_0200632c[] = {
    0,
};

u32 gData_02006330[] = {
    38,
};
#undef ACTOR_MODE_THEN_WAIT
#undef FOLLOW_LEADER
#undef FIELD_AT
