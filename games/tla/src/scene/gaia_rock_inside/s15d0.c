/// @file
/// Scene code of overlay 0x681, in ROM order, from s15d0's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "system/io_reg.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "field_effect.h"
#include "system/field_effect.h"
#include "fixed_math.h"

struct Spark {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x0f];
  u8 priorityFlags;
  u8 unk24[0x0c];
  s32 speedLimit;
  s32 acceleration;
  u8 unk38[0x0c];
  s32 bounce;
  s32 gravity;
  s32 unk4c;
  u8 unk50[5];
  u8 motionFlags;
  u8 unk56[0x0c];
  u8 unk62;
  u8 unk63;
  s16 action;
  u16 unk66;
  struct Actor *watchedObject;
};

extern const u8 gData_02005264[], gData_02005218[], gData_02005234[],
    gData_02005288[], gData_02005244[], gData_020052ac[], gData_020052dc[];
u32 Random16Far(void);
void Scene_SetActorSpritePriority(void *object, s32 priority);
void Func_02004d9c(void *object, s32 mode);
void Actor_SetSingleChildFlags(void *object, u32 value);
void Animation_SetStateFlags(void *object, s32 flags);
void Func_02004dac(void *object, const u8 *script);
void Func_02001548(void);
void Func_02004fd4(void);

void Func_020015d0(s32 idA, s32 idB) {
  struct Actor *a = Actor_GetById(idA);
  struct Actor *b = Actor_GetById(idB);
  s32 i;
  s16 *angle;
  s32 kind = 0x2a0;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  Sound_Play(78);
  Event_Wait(30);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&a->x)[1] << 16, ((s16 *)&a->y)[1] << 16,
                             ((s16 *)&a->z)[1] << 16, 1);
  Sound_Play(141);
  /* FAKEMATCH(wrapper): forwarded, so each argument builds its own 0x20000; as
   * a direct call, 17 of 402 instructions differ in tla-en. */
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(60);
  a->motionFlags = 0;
  a->acceleration = 0x3333;
  a->speedLimit = 0x3333;
  Actor_SetMovementTypeById(idA, gData_02005264);
  Sound_Play(194);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(idA, 0x100);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
first:
  {
    struct Spark *spark = ((void *)Import_ActorCreate(kind, a->x, a->y, a->z));
    Sound_Play(246);
    spark->x += ((Random16Far() & 31) - 16) << 16;
    spark->y += ((Random16Far() & 15) - 8) << 16;
    spark->speedLimit = 0x80000;
    spark->acceleration = 0x3333;
    spark->motionFlags = 0;
    Actor_SetPartAttribute(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Func_02004d9c(spark, 7);
    Actor_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Func_02004dac(spark, gData_02005218);
    REG_BLDALPHA = 0x1004;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
    REG_BLDALPHA = 0x1010;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
  }
  i++;
  if (i != 16)
    goto first;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
second:
  {
    struct Spark *spark = ((void *)Import_ActorCreate(kind, b->x, b->y, b->z));
    Sound_Play(195);
    angle = &spark->action;
    *angle = (Random16Far() & 0x7fff) + 0x8000;
    spark->unk66 = ((i & 1) * 2 - 1) << ((Random16Far() & 3) + 9);
    spark->motionFlags = 0;
    spark->unk4c = 0x7a0000;
    spark->watchedObject = b;
    spark->unk62 = (16 - i) * 8 + 15;
    Actor_SetPartAttribute(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Func_02004d9c(spark, 7);
    Actor_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2
     * of 402 instructions differ in tla-en. */
    FieldVector_AddPolarOffset(spark->unk4c, *angle, &spark->x);
    Func_02004dac(spark, gData_02005234);
    if (i == 3) {
      /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call,
       * 2 of 402 instructions differ in tla-en. */
      Actor_SetSpriteColorById(idB, 0x100);
      b->motionFlags = 0;
      b->acceleration = 0x3333;
      b->speedLimit = 0x3333;
      /* FAKEMATCH(barrier): references to idA rank it over idB and hold idB in
       * r0 before the script load; as no asm, 22 of 402 instructions differ in
       * tla-en. */
      asm("" : : "r"(idA));
      Actor_SetMovementTypeById(idB, gData_02005288);
    }
    wait(8);
  }
  i++;
  if (i != 16)
    goto second;
  Sound_Play(220);
  wait(16);
  i = 2;
third:
  {
    struct Spark *spark = ((void *)Import_ActorCreate(
        kind, b->x, (i << 16) + b->y - 0x80000, b->z));
    spark->action = Random16Far();
    spark->unk66 = 0x800;
    spark->unk62 = i;
    spark->unk63 = 1;
    spark->bounce = spark->x;
    spark->unk4c = 0;
    spark->gravity = spark->z;
    spark->watchedObject = b;
    spark->motionFlags = 0;
    spark->priorityFlags = 1;
    Actor_SetPartAttribute(spark, 2);
    Func_02004d9c(spark, 7);
    Actor_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Func_02004dac(spark, gData_02005244);
  }
  i += 2;
  if (i != 32)
    goto third;
  Sound_Play(220);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(idA, 0);
  Actor_ConfigureSpriteColor(idB, 0);
  Sound_Play(574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(80);
  Actor_SetMovementTypeById(idA, gData_020052ac);
  Actor_SetMovementTypeById(idB, gData_020052dc);
  b->update = Func_02001548;
  wait(120);
  b->update = 0;
  Sound_Play(390);
  Func_02004fd4();
  wait(1);
}

// The party meets person 11 on the ledge: they talk, the ground shakes, the screen blends out and back while the ledge changes, the party gains the item and person 11 joins the leader (flag 0x9ba).
void Func_02004e5c(void);
void Func_02004ec4(s32 id, s32 target);
void Func_02004ddc(void);
void Func_02004dd4(void);
void Party_LearnAbility(s32 unit, s32 ability);
s32 Func_02004e64(void);

s32 Func_020019cc(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  /* FAKEMATCH(volatile): the blend level rereads through its slot; as a plain local,
   * 112 of 250 instructions differ in tla-en. */
  volatile u16 level;
  s32 message;

  /* FAKEMATCH(wrapper): the calls that repeat an argument are forwarded through
   * call.h, so each builds its own; as direct calls, 32 of 250 instructions differ
   * in tla-en. */
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  message = (s32)MsgGaiaRockInside09;
  ((s32 (*)())FieldMessage_Show)(message, 1);
  Actor_ShowEmote(gMainRoutineWork.currentOwner, 0x100, 0);
  if (leader->z >> 19 <= 39) {
    Actor_WalkTo(gMainRoutineWork.currentOwner, 0x1b8, 0x148);
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  }
  Func_02004ec4(11, gMainRoutineWork.currentOwner);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 0x1d6, 0x162);
  Actor_WalkTo(11, 0x1be, 0x162);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  FieldActor_WaitForMoveById(11);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(11, 0, 0);
  FieldEvent_SetMessageId(message + 1);
  Event_ShowMessage(11, 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0x8000, 0);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(10);
  Actor_SetAnimationAndWait(11, 3);
  FieldEvent_Wait(10);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, 0xc000, 0);
  Actor_FaceDirection(11, 0xc000, 0);
  FieldEvent_Wait(10);
  Actor_WalkTo(gMainRoutineWork.currentOwner, 0x1d2, 0x156);
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Message_ShowCentered(message + 2, 1);
  MapGrid_SetShake(0x50000, 0x70000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
  Func_02004ddc();
  level = REG_BLDALPHA & 0xff;
  while (level != 0) {
    REG_BLDALPHA = level | 0x1000;
    level--;
    System_Wait(1);
  }
  REG_DISPCNT &= ~0x200;
  Func_020015d0(12, 4);
  REG_DISPCNT |= 0xf00;
  REG_BLDALPHA = level | 0x1000;
  REG_BLDCNT = 0x3f42;
  while (level != 8) {
    REG_BLDALPHA = level | 0x1000;
    level++;
    System_Wait(1);
  }
  Func_02004dd4();
  Actor_SetPosition(12, 0, 0);
  Party_LearnAbility(4, 139);
  Event_SetFlag(0x9ba);
  Actor_SetSpeed(11, 0x13333, 0x9999);
  Actor_SetAnimation(11, 2);
  {
    struct Actor *owner = Actor_Get(gMainRoutineWork.currentOwner);

    if (owner != 0)
      Actor_SetDestination(11, ((s16 *)&owner->x)[1], ((s16 *)&owner->z)[1]);
  }
  FieldActor_WaitForMoveById(11);
  Actor_SetPosition(11, 0, 0);
  FieldEvent_Wait(20);
  return Func_02004e64();
}

void Effect_AdvanceMotionLocal(union FieldObject *object) {
  struct FieldEffect *effect = &object->effect;
  s32 velocityX;
  s32 velocityZ;
  s32 scaleRateX;
  struct FieldSprite *sprite;

  velocityX = effect->velocity.x;
  effect->x += velocityX;
  effect->y += effect->velocity.y;
  velocityZ = effect->velocity.z;
  effect->z += velocityZ;

  effect->velocity.x = velocityX - velocityX / 18;
  effect->velocity.z = velocityZ - velocityZ / 16;

  scaleRateX = effect->scaleRateX;
  effect->scaleX += scaleRateX;
  sprite = effect->sprite;
  effect->scaleY += effect->scaleRateY;
  sprite->rotation += effect->spin;
}

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
s32 Math_Cosine(s32 angle);

void Func_02001c9c(void) {
  s32 velocity[3];
  struct EffectOptions options;
  u32 i;

  options.update = Effect_AdvanceMotionLocal;
  i = 0;
  do {
    {
      s32 angle = i << 12;

      velocity[0] = Math_Cosine(angle);
      velocity[1] = 0;
      velocity[2] = Math_Sin(angle);
      velocity[0] += velocity[0] / 3;
      Func_0200015c(0x1d00000, -0x200000, 0x1480000, velocity[0], velocity[1],
                    velocity[2], EFFECT_USE_UPDATE | 1, &options);
      i += 2;
    }
  } while (i <= 16);
}

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004dbc(struct Actor *object);

/// Unless flag 0x9ee or 0x9ba is set: fades the blend out, lowers person 12 by
/// one unit every 2 frames for 20 steps, with Func_02001c9c's ring at the start
/// and halfway, places object 323, sets flag 0x9ee, and fades the blend back.
void Func_02001d14(void) {
  struct Actor *person;
  struct Actor *object;
  /* FAKEMATCH(volatile): the form kept; as u16 level, 146 of 173 instructions
   * differ in tla-en. */
  volatile u16 level;
  s32 i;

  if (GameFlag_Test(0x9ee) || GameFlag_Test(0x9ba))
    return;
  person = Actor_Get(12);
  level = REG_BLDALPHA;
  Func_02004e5c();
  FieldEvent_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): the form kept; as a direct call, 13 of 173
   * instructions differ in tla-en. */
  MapGrid_SetShake(Q16_ONE * 10, Q16_ONE * 10, Q16_ONE);
  Event_PlaySound(107);
  Func_02004ddc();
  level &= 0xff;
  while (level != 0) {
    REG_BLDALPHA = level | 0x1000;
    level--;
    System_Wait(1);
  }
  REG_DISPCNT &= ~0x200;
  FieldEvent_Wait(30);
  object =
      OverlayObject_PrepareObject(232 << 17, -Q16_ONE * 32, 174 << 17, 323);
  Actor_SetPosition(12, 232 << 17, 174 << 17);
  Func_02001c9c();
  for (i = 0; i <= 19; i++) {
    if (i == 10)
      Func_02001c9c();
    person->z -= Q16_ONE;
    FieldEvent_Wait(2);
  }
  Event_SetFlag(0x9ee);
  /* FAKEMATCH(wrapper): the form kept; as a direct call, 25 of 173
   * instructions differ in tla-en. */
  MapGrid_SetShake(-1, -1, 0xe666);
  object->x = 0;
  object->z = 0;
  FieldEvent_Wait(1);
  Func_02004dbc(object);
  Event_PlaySound(390);
  FieldEvent_Wait(30);
  REG_DISPCNT |= 0xf00;
  REG_BLDALPHA = level | 0x1000;
  REG_BLDCNT = 0x3f42;
  while (level != 8) {
    REG_BLDALPHA = level | 0x1000;
    level++;
    System_Wait(1);
  }
  Func_02004dd4();
  Func_02004e64();
}

// Leaving Gaia Rock: after flag 0x927 a message; otherwise sets the next
// destinations (scene 0xb1, entrance 10, and 11 or 12 by the item 0x1c3 and
// the four flags 0x9bc to 0x9bf) and leaves. The destination setters take s32:
// as u16 the scene symbol loads through ldrh and the pools break up.
#include "types.h"
#include "main_routine_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

extern char SceneId_Ov681Room1;

s32 Func_02004f74(s32 entrance, s32 count);

void Func_02001ebc(void) {
  s32 count;

  if (Event_GetFlag(0x927)) {
    Message_ShowCentered((s32)MsgGaiaRockInside08, 1);
  } else {
    s32 a = Event_GetFlag(0x9bc);
    s32 b = Event_GetFlag(0x9bd);
    s32 c = Event_GetFlag(0x9be);
    s32 scene;

    count = a + b + c + Event_GetFlag(0x9bf);
    /* FAKEMATCH(dead): the count is summed before the battle rule store;
     * without it, 8 of 68 instructions differ in tla-en. */
    do {
    } while (0);
    gMainRoutineWork.battleRule24b = 2;
    /* FAKEMATCH(dead): the scene address is loaded after the battle rule
     * store; without it, 4 of 68 instructions differ in tla-en. */
    do {
    } while (0);
    scene = (s32)&SceneId_Ov681Room1;
    Event_SetAfterBattleDestination(scene, 10);
    if (FieldPartyInventory_FindOwner(0x1c3) != -1) {
      if (count == 0)
        Event_SetDefeatDestination(scene, 12);
    } else {
      Event_SetDefeatDestination(scene, 11);
    }
    Func_02004f74(12, count);
  }
}
