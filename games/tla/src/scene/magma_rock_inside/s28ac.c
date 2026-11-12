#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"

struct FieldActor;

void Func_02004d40(void);
void Func_02004d48(void);
void Func_02004e20(void);
struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004cb0(struct FieldActor *actor);
s32 Func_02000694(u32 layer, s32 x, s32 y, s32 value);

void Func_020028ac(void) {
  struct Actor *object;
  struct FieldActor *actor;
  struct Actor *other;
  s32 i;

  object = Actor_Get(((s8 *)Ram_HeapSlots->eventWork)[0xd30]);
  Actor_SetSpriteColor(object, 0);
  Event_PlaySound(136);
  i = 0;
  do {
    object->scaleY = object->scaleX += 0x1000;
    System_Wait(5);
    i++;
  } while (i != 16);
  Event_SetFlag(0x305);
  other = Actor_Get(9);
  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(0x20000, 0x10000, 0x10000);
  Event_PlaySound(141);
  FieldEvent_Wait(30);
  FieldCamera_MoveTo(0x780000, -1, 0x1580000, 1);
  Func_02004e20();
  FieldEvent_Wait(30);
  Event_PlaySound(0x22d);
  REG_BLDALPHA = 0x1000;
  REG_BLDCNT = 0x3f10;
  {
    struct FieldActor *prepared = OverlayObject_PrepareObject(
        other->x, other->terrainHeight, other->z + 0x300000, 312);

    other->motionFlags = 0;
    actor = prepared;
  }
  i = 0;
  do {
    other->y -= 0x10000;
    FieldEvent_Wait(5);
    i++;
  } while (i != 32);
  Event_PlaySound(0x22a);
  Func_02004c98(other, 4);
  Func_02004cb0(actor);
  FieldEvent_Wait(1);
  REG_BLDCNT = 0;
  Func_02000694(0, 7, 21, 128);
  Func_02004d48();
}

#include "local.h"
#include "types.h"
#include "event_inline.h"
#include "frame.h"
#include "actor.h"
#include "field_effect.h"
#include "call.h"
#include "event_work.h"
#include "edition.h"
#include "main_routine_work.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_020029dc(void) {
  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 0xd30) == -1) {
    if (!Event_GetFlag(0x305))
      Actor_SetPosition(11, 0, 0);
  }
}

void Func_02002a14(struct Actor *source) {
  struct Actor *object = Create(0x2a0, source->x, source->y, source->z);

  object->scaleY = 0x6666;
  object->scaleX = 0x6666;
  object->motionFlags = 0;
  Actor_SetSpriteColor(object, 10);
  Scene_SetActorSpritePriority(object, 1);
  Func_02004c98(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Func_02004ca0(object, gData_02005534);
}

struct Orbiter {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x38];
  s32 radius;
  u8 unk50[0x12];
  u8 timer;
  u8 unk63;
  u16 angle;
  u16 speed;
  struct Orbiter *parent;
};

s32 Func_02002a6c(struct Orbiter *object) {
  u16 *angle = &object->angle;
  u16 *speed = &object->speed;
  struct Orbiter *parent = object->parent;
  s32 *position;
  s16 heading;

  object->angle += (s16)*speed >> 1;
  object->x = parent->x;
  object->y = parent->y + 0x80000;
  object->z = parent->z;
  heading = *angle;
  FieldVector_AddPolarOffset(object->radius, heading, position = &object->x);
  if (--object->timer == 0)
    return 0;
  if (object->radius == 0x20000)
    return 1;
  Func_02002a14(object);
  object->radius -= 0x40000;
  object->angle += (s16)*speed >> 1;
  *position = parent->x;
  object->y = parent->y + 0x80000;
  object->z = parent->z;
  FieldVector_AddPolarOffset(object->radius, (s16)*angle, position);
  Func_02002a14(object);
  object->radius -= 0x40000;
  return 1;
}

struct Orbit {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x30];
  s32 centreX;
  s32 centreZ;
  s32 radius;
  u8 unk50[0x12];
  u8 phase;
  u8 growth;
  s16 angle;
  s16 speed;
};

s32 Math_Cosine(s32 angle);
// Called by its linked name, as Gaia Rock's Func_0200142c does.
s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

// Moves around in a circle.
s32 Func_02002b2c(struct Orbit *object) {
  s32 z;

  object->angle += object->speed;
  object->x = IwramMulQ16(Math_Cosine(object->angle), object->radius);
  z = IwramMulQ16(Math_Sin(object->angle), object->radius);
  object->x += object->centreX;
  object->z = (z >> 1) + object->centreZ;
  if (object->growth != 141 && (gFrameCount & 1)) {
    object->radius = object->growth * Math_Sin(object->phase << 10);
    object->growth += 10;
  }
  if (object->speed == 0)
    return 0;
  if (object->growth == 141)
    object->speed -= 128;
  return 1;
}

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

s32 Func_02002bd0(struct Actor *object) {
  struct Actor *target = object->watchedObject;
  s32 steps;
  s32 from;

  object->bounce = (target->x - object->x) / (steps = 10);
  /* FAKEMATCH(dead): an empty once-only block, the first fence of a pair, so
   * the x step is stored, then the object height read, then the target
   * height (with no block the target height is read first); as no block, 2 of
   * 35 instructions differ in tla-en. */
  do {
  } while (0);
  from = object->y;
  /* FAKEMATCH(barrier): an empty asm, the second fence of the pair above; as
   * no asm, 2 of 35 instructions differ in tla-en. */
  asm volatile("");
  object->unk4c = (target->y - from + 0x100000) / steps;
  object->gravity = (target->z - object->z) / steps;
  BYTE_AT(object, 98) = steps;
  return 0;
}

s32 Func_02002c1c(struct Actor *object) {
  u8 *steps;
  s32 v;
  u8 n;

  object->x += object->bounce;
  object->y += object->unk4c;
  object->z += object->gravity;
  steps = (u8 *)object + 98;
  v = *steps + 255;
  *steps = v;
  n = v;
  return n != 0;
}

u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02002c48(struct Actor *object) {
  struct EffectOptions options;
  s32 position[3];
  u32 frame = gFrameCount & 7;

  if (frame != 0)
    return;
  options.palette = 7;
  options.type = 673;
  options.priority = 1;
  position[0] = object->x + (((Random16Far() & 31) - 16) << 16);
  position[1] = object->y + ((Random16Far() & 31) << 16) + 0x80000;
  position[2] = object->z;
  Func_0200015c(position[0], position[1], position[2] + 0x40000, 0, 0x8000,
                frame, 0x130000, &options);
}

union ActorView {
  struct Actor actor;
  struct {
    u8 unk00[0x62];
    u8 unk62;
    u8 unk63;
  } bytes;
};

extern const u8 gData_02005590[];
extern const s32 gData_02005544[];
extern const s32 gData_02005560[];
extern const u8 gData_020055b4[];
extern const s32 gData_02005570[];
extern const u8 gData_020055d8[];
extern const u8 gData_02005608[];

void Func_02004ea0(void);

void Func_02002cd0(s32 a, s32 b) {
  struct Actor *first = Actor_GetById(a);
  struct Actor *second = Actor_GetById(b);
  struct Actor *object;
  struct Actor *spark;
  struct Actor *riser;
  s32 i;
  u16 noMotion;
  s32 probe;
  const u8 *script;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  script = gData_020055b4;
  Sound_Play(78);
  Event_Wait(30);
  FieldCamera_SetSpeed(0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&first->x)[1] << 16,
                             ((s16 *)&first->y)[1] << 16,
                             ((s16 *)&first->z)[1] << 16, 1);
  Sound_Play(141);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  BattleEffect_ApplyColorToSourceBuffer(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 1);
  BattleEffect_StartBufferInterpolation(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetMovementTypeById(a, gData_02005590);
  Sound_Play(194);
  Event_Wait(45);
  Actor_ConfigureSpriteColor(a, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  object = (struct Actor *)(struct Actor *)Import_ActorCreate(
      672, first->x, first->y, first->z);
  Sound_Play(246);
  object->x += ((Random16Far() & 31) - 16) << 16;
  object->y += ((Random16Far() & 15) - 8) << 16;
  object->speedLimit = 0x80000;
  object->acceleration = 0x3333;
  object->motionFlags = 0;
  Actor_SetPartAttribute(object, 10);
  Scene_SetActorSpritePriority(object, 1);
  Func_02004c98(object, 7);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Func_02004ca0(object, gData_02005544);
  REG_BLDALPHA = 0x1004;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  REG_BLDALPHA = 0x1010;
  wait(2);
  REG_BLDALPHA = 0x100a;
  wait(2);
  i++;
  if (i != 16)
    goto burst;
  REG_BLDALPHA = 0x1008;
  wait(30);
  i = 0;
  /* FAKEMATCH(dead): the never-taken jump into the spiral keeps its refs at
   * weight 1; as the spiral alone, 174 of 402 instructions differ in tla-en. */
  probe = second->unk57;
  if (probe & 0x100) {
    wait(1);
    goto tail;
  }
spiral:
  spark = FieldActor_Create(672, second->x, second->y, second->z);
  Sound_Play(195);
  spark->action = (Random16Far() & 0x7fff) + 0x8000;
  spark->unk66 = (((i & 1) << 1) - 1) << ((Random16Far() & 3) + 9);
  noMotion = 0;
  spark->motionFlags = noMotion;
  spark->unk4c = 0x7a0000;
  spark->watchedObject = second;
  ((union ActorView *)spark)->bytes.unk62 = ((16 - i) << 3) + 15;
  Actor_SetPartAttribute(spark, 10);
  Scene_SetActorSpritePriority(spark, 1);
  Func_02004c98(spark, 7);
  Actor_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  FieldVector_AddPolarOffset(spark->unk4c, spark->action, &spark->x);
  Func_02004ca0(spark, gData_02005560);
  if (i == 3) {
    Actor_SetSpriteColorById(b, 256);
    second->motionFlags = noMotion;
    second->acceleration = 0x3333;
    second->speedLimit = 0x3333;
    Actor_SetMovementTypeById(b, script);
  }
tail:
  wait(8);
  i++;
  if (i != 16)
    goto spiral;
  Sound_Play(220);
  wait(16);
  i = 2;
column:
  riser = FieldActor_Create(672, second->x, second->y + i * 0x10000 - 0x80000,
                            second->z);
  riser->action = Random16Far();
  riser->unk66 = 0x800;
  ((union ActorView *)riser)->bytes.unk62 = i;
  ((union ActorView *)riser)->bytes.unk63 = 1;
  riser->bounce = riser->x;
  riser->unk4c = 0;
  riser->gravity = riser->z;
  riser->watchedObject = second;
  riser->motionFlags = (u16)0;
  riser->priorityFlags = 1;
  Actor_SetPartAttribute(riser, 10);
  Func_02004c98(riser, 7);
  Actor_SetSingleChildFlags(riser, 0);
  Animation_SetStateFlags(riser, 1);
  Func_02004ca0(riser, gData_02005570);
  i += 2;
  if (i != 32)
    goto column;
  Sound_Play(220);
  wait(50);
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(8);
  wait(16);
  MapGrid_SetShake(0, 0, 0);
  Actor_ConfigureSpriteColor(a, 0);
  Actor_ConfigureSpriteColor(b, 0);
  Sound_Play(574);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(80);
  Actor_SetMovementTypeById(a, gData_020055d8);
  Actor_SetMovementTypeById(b, gData_02005608);
  second->update = (void (*)(void))Func_02002c48;
  wait(120);
  second->update = 0;
  Sound_Play(390);
  Func_02004ea0();
  wait(1);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

extern const u8 MsgBlazeTabletInscription[];

s32 Func_02004da8();
s32 Party_LearnAbility();

/// Reading the Blaze tablet shows its inscription; the first time (flag 0x9fa
/// clear) the scene with actor 5 follows.
void Func_020030cc(void) {
  struct Actor *object;

  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgBlazeTabletInscription, 1);
  if (Event_GetFlag(0x9fa) != 0) {
    Func_02004d48();
  } else {
    FieldEvent_SetMessageId((s32)MsgBlazeTabletInscription);
    EVENT_WORK->messageId++;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0xb333, 0x5999);
    Actor_SetSpeed(5, 0xb333, 0x5999);
    Func_02004da8(5, gMainRoutineWork.currentOwner);
    FieldEvent_Wait(1);
    Actor_WalkTo(gMainRoutineWork.currentOwner, 256, 136);
    Actor_WalkTo(5, 272, 136);
    FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
    FieldActor_WaitForMoveById(5);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_FaceDirection(5, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Event_ShowMessage(5, 0);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(30);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 256, 152);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_FaceDirection(5, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetAnimation(5, 3);
    FieldEvent_Wait(30);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_WalkToAndWait(5, 264, 137);
    Actor_FaceDirection(5, FACING_NORTH, 0);
    FieldEvent_Wait(20);
#if (LANGUAGE == LANGUAGE_JAPANESE) || (LANGUAGE == LANGUAGE_ENGLISH)
    Message_ShowCentered(MsgBlazeTabletInscription + 3, 1);
#else
    Message_ShowCentered(MsgBlazeTabletInscription + 2, 1);
#endif
    MapGrid_SetShake(0x50000, 0x70000, 0x10000);
    MapGrid_SetShake(-1, -1, 0xe666);
    MapGrid_WaitForShakeToSettle();
    Call2(Func_02002cd0, 8, 5);
    Party_LearnAbility(5, 0x9a);
    Event_SetFlag(0x9fa);
    Actor_FaceEachOther(gMainRoutineWork.currentOwner, 5, 0);
    FieldEvent_Wait(10);
    Actor_SetAnimation(5, 3);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Actor_SetSpeed(5, 0x13333, 0x9999);
    Actor_SetAnimation(5, 2);
    object = Actor_Get(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_SetDestination(5, FIELD_AT(object, s16, 10),
                           FIELD_AT(object, s16, 18));
    FieldActor_WaitForMoveById(5);
    Actor_SetPosition(5, 0, 0);
    Actor_SetActionCallback(Actor_Get(8), 9);
    Func_02004d48();
  }
}

void Func_020032d8(void) {
  if (Event_GetFlag(0x9fa)) {
    Func_02004d40();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 107);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 136);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}

void Func_02003348(void) {
  if (Event_GetFlag(0x9fa)) {
    Func_02004d40();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 280, 136);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}

void Func_020033b0(void) {
  if (Event_GetFlag(0x9fa)) {
    Func_02004d40();
    FieldEvent_ResolvePendingActions(0);
    Message_ShowCentered(MsgBlazeTabletInscription, 1);
    Func_02004d48();
  } else {
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 248, 136);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 264, 136);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_020030cc();
  }
}

extern u8 gData_02005f60[];
extern u8 gData_02005fe4[];
extern u8 gData_020060bc[];
extern u8 gData_020061c4[];
extern u8 gData_020063a4[];
extern u8 gData_02006458[];
extern u8 gData_0200659c[];
extern u8 gData_02006714[];

u8 *Func_02003418(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene != (s32)&SceneId_Ov692Room1) {
    if (scene == (s32)&SceneId_Ov692Room2)
      return gData_02005fe4;
    if (scene == (s32)&SceneId_Ov692Room3)
      return gData_020060bc;
    if (scene == (s32)&SceneId_Ov692Room4)
      return gData_020061c4;
    if (scene == (s32)&SceneId_Ov692Room5)
      return gData_020063a4;
    if (scene == (s32)&SceneId_Ov692Room6)
      return gData_02006458;
    if (scene == (s32)&SceneId_Ov692Room7)
      return gData_0200659c;
    if (scene == (s32)&SceneId_Ov692Room8)
      return gData_02006714;
  }
  return gData_02005f60;
}

void Func_020034bc(void) {
  s32 i;

  Event_SetFlag(0x300);
  Func_02000694(0, 15, 13, 0);
  for (i = 0; i != 6; i++)
    Actor_SetAnimation(i + 9, 2);
  Actor_SetAnimation(15, 4);
}

void Func_020034f0(void) {
  Event_SetFlag(0x301);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room3) {
    Func_02000694(0, 24, 15, 0);
    Func_02000694(0, 9, 16, 0);
    Actor_SetAnimation(11, 4);
  } else {
    Func_02000694(0, 26, 14, 0);
  }
}

extern s16 gData_0200525c[];
extern s16 gData_020052bc[];
extern s16 gData_0200531c[];
extern s16 gData_0200537c[];

void Func_02000800(s32 id, s16 *script, s32 flag);

void Func_02003548(void) {
  // Event_SetFlag, event.h's command, keeps 0x302 from being shared with the
  // script calls, which build it again after the scene test; GameFlag_SetBit
  // called directly differs in 56 instructions.
  Event_SetFlag(0x302);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov692Room4) {
    s16 *script = gData_0200525c;

    Func_02000800(15, script, 0x302);
    Func_02000800(16, gData_0200525c, 0x302);
    Func_02000800(17, gData_020052bc, 0x302);
    Func_02000800(18, gData_020052bc, 0x302);
    Func_02000800(19, gData_0200531c, 0x302);
    Func_02000800(20, gData_0200531c, 0x302);
    Func_02000800(21, gData_0200537c, 0x302);
    Func_02000800(22, gData_0200537c, 0x302);
    if (Actor_Get(16)->step == 0) {
      Actor_Get(16)->step = 10;
      Actor_Get(18)->step = 10;
      Actor_Get(20)->step = 10;
      Actor_Get(22)->step = 10;
    }
    Actor_SetAnimation(23, 4);
    Actor_SetAnimation(24, 4);
    Actor_SetAnimation(25, 4);
    Actor_SetAnimation(26, 2);
  }
}

void Func_02003630(struct Actor *arg) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *object;
  struct Actor *owner;
  s16 ownerY;
  s16 y;
  u8 *flags;

  do {
  } while (0);
  object = arg;
  owner = Actor_Get(party->currentOwner);
  ownerY = owner->y >> 16;
  y = object->y >> 16;
  flags = &object->priorityFlags;

  if (ownerY != y || (s16)(owner->z >> 16) < (s16)(object->z >> 16)) {
    *flags = 1;
  } else {
    *flags = 0;
    Scene_SetActorSpritePriority(object, 3);
  }
}
