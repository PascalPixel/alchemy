/// @file
/// Scene code of overlay 0x664, in ROM order, from s3ac4's offset up to the next object the overlay links.

#include "airs_rock_inside.h"
#include "system.h"

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02005e84(s32 owner, s32 a, s32 b);
void Func_02005efc(s32 countdown);

void Func_02003ac4(void) {
  Func_02005e14();
  Func_02005f2c(0);
  OverlayObject_PrepareObject(0xd80000, -0x200000, 0x2200000, 286);
  Func_02005e84(gMainRoutineWork.currentOwner, 6, 0);
  Func_02005efc(3);
  Func_02005e1c();
}

void Actor_SetSpeedParameters(u32 objectId, s32 speedLimit, s32 acceleration);
void Actor_SetPositionAndCommit(u32 objectId, s32 x, s32 z);

/// While the leader is no further than z 653, moves the leader to (376, 668)
/// with bit 0 of its byte 90 cleared for the move.
void Func_02003b08(void) {
  u8 *flags;
  u8 bit;

  if (((s16 *)&Actor_GetById(gMainRoutineWork.currentOwner)->z)[1] > 653)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(186);
  BYTE_AT(Actor_GetById(gMainRoutineWork.currentOwner), 90) &= ~1;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x40000, 0x20000);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 376, 668);
  flags = &BYTE_AT(Actor_GetById(gMainRoutineWork.currentOwner), 90);
  /* FAKEMATCH(temporary): the bit held in a u8 local, built in the register it
   * ORs into, as the ROM does; as |= 1, the byte loads there instead, 4 of
   * 47 instructions differ in tla-en (also as an s32 local). */
  bit = 1;
  *flags = bit | *flags;
  Func_02005e1c();
}

#include "types.h"
#include "call.h"
#include "actor.h"
#include "field_sprite.h"
#include "system/io_reg.h"

void Func_0200062c(s32 layer, s32 x, s32 z, s32 cell);
void Func_02005d7c(struct Actor *object);

void Func_02003b7c(s32 x, s32 y) {
  struct Actor *marker;
  struct FieldSprite *sprite;
  s32 i, cellX, cellY;

  marker = Import_ActorCreate(288, x << 16, 0, y << 16);
  REG_BLDALPHA = 16;
  REG_BLDCNT = 0x3f44;
  Scene_SetActorSpritePriority(marker, 0);
  Actor_SetSingleChildFlags(marker, 0);
  sprite = marker->sprite;
  sprite->blendMode = 2;
  Func_02005e14();
  Func_02005f2c(0);
  REG_WINOUT = 0x3f1f;
  REG_DISPCNT |= 0x8000;
  Func_02005f94(106);
  i = 0;
fadeIn:
  REG_BLDALPHA = (i << 9) | 16;
  wait(1);
  i++;
  if (i != 8)
    goto fadeIn;
  i = 0;
fadeOut:
  REG_BLDALPHA = (16 - i) | 0x1000;
  wait(1);
  i++;
  if (i != 16)
    goto fadeOut;
  cellX = x >> 4;
  cellY = (y >> 4) - 1;
  Call6((void (*)())Func_02005d9c, 65, 1, cellX + 64, cellY, 1, 2);
  Func_0200062c(0, cellX, cellY, 0xff00);
  Func_02005d7c(marker);
  Func_02005e1c();
  REG_BLDCNT = 0;
  REG_DISPCNT &= 0x7fff;
}

#include "main_routine_work.h"
#include "ram_buffer.h"

s32 Func_02000690(s32 kind, s32 x, s32 z);
void Func_02002790(void);

/// Reads what the leader stands on (Func_02000690, kind 2): on cells 1 and 7
/// it only redraws the marker when flag 0x200 is set; elsewhere it clears
/// (Func_02002790), redraws the marker in the row the event work's counter
/// gives, sets flag 0x200 and the counter's flag (312 on).
void Func_02003cac(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  struct Actor *leader;
  s32 cell, row = 54;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  cell = Func_02000690(2, leader->x >> 20, leader->z >> 20);
  work += 0x16a;
  // From 54 in two steps: as one expression the constants fold into
  // 858 - 4 * counter.
  row -= (*(s16 *)work - 201) << 2;
  cell >>= 8;
  if (cell != 1 && cell != 7) {
    Func_02002790();
    Func_02003b7c(0x108, row * 8 + 10);
    if (!Value1((s32 (*)())GameFlag_Test, 0x200))
      GameFlag_SetBit(0x200);
    GameFlag_SetBit(*(s16 *)work + 312);
  } else if (GameFlag_Test(0x200)) {
    Func_02003b7c(0x108, row * 8 + 10);
  }
}

/// As Func_02002980 with a 1 by 2 block: saves the cells 64 columns across
/// from the controlled object at (73, 72) and stamps the cells kept at
/// (70, 74) in their place.
void Func_02003d40(void) {
  struct Actor *object = Actor_GetById(Func_02005f84());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gData_02006c0c) {
    gData_02006c0c = key;
    gData_020073dc = 1;
    gData_020073e0 = 2;
    gData_020073e4 = x + 64;
    gData_020073e8 = z - y;
    CopyCells(gData_020073e4, gData_020073e8, 1, 2, 73, 72);
    CopyCells(70, 74, gData_020073dc, gData_020073e0, gData_020073e4,
              gData_020073e8);
  }
}

extern const u8 gData_02006084[];

void Func_02003dd4(void) {
  Func_02005dac(26, 17, 1, 1, 8, 19);
  Func_02005dac(26, 17, 1, 1, 24, 19);
  Call6((void (*)())Func_02005d9c, 90, 17, 72, 19, 1, 2);
  Func_02005d9c(90, 17, 88, 19, 1, 2);
}

void Func_02003e28(void) {
  Func_02005dac(25, 17, 1, 1, 8, 19);
  Func_02005dac(25, 17, 1, 1, 24, 19);
  Call6((void (*)())Func_02005d9c, 89, 17, 72, 19, 1, 2);
  Func_02005d9c(89, 17, 88, 19, 1, 2);
}

void Func_02003e7c(struct Actor *source) {
  struct Actor *object = (struct Actor *)Value4(
      (s32 (*)())Import_ActorCreate, 0x2a0, source->x, source->y, source->z);

  WORD_AT(object, 28) = 0x6666;
  WORD_AT(object, 24) = 0x6666;
  object->motionFlags = 0;
  Actor_SetPartAttribute(object, 2);
  Scene_SetActorSpritePriority(object, 1);
  Func_02005d5c(object, 1);
  Actor_SetSingleChildFlags(object, 0);
  Animation_SetStateFlags(object, 1);
  Func_02005d6c(object, gData_02006084);
}

struct Orbiter {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x38];
  s32 radius;
  u8 unk50[0x12];
  u8 countdown;
  u8 unk63;
  s16 angle;
  u16 speed;
  struct Actor *anchor;
};

void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);

s32 Func_02003ed4(struct Orbiter *orbiter) {
  struct Actor *anchor = orbiter->anchor;

  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + 0x80000;
  orbiter->z = anchor->z;
  Call3((void (*)())Vector_AddPolarOffsetFar, orbiter->radius, orbiter->angle,
        (s32)&orbiter->x);
  if (--orbiter->countdown == 0)
    return 0;
  if (orbiter->radius == 0x20000)
    return 1;
  Func_02003e7c(orbiter);
  orbiter->radius -= 0x40000;
  orbiter->angle += (s32)(orbiter->speed << 16) >> 17;
  orbiter->x = anchor->x;
  orbiter->y = anchor->y + 0x80000;
  orbiter->z = anchor->z;
  Vector_AddPolarOffsetFar(orbiter->radius, orbiter->angle, &orbiter->x);
  Func_02003e7c(orbiter);
  orbiter->radius -= 0x40000;
  return 1;
}

#include "frame.h"

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
s32 Math_Sine(s32 angle);
// Called by its linked name, as Gaia Rock's Func_0200142c does.
s32 IwramMulQ16(s32 a, s32 b) __attribute__((long_call));

// Moves around in a circle.
s32 Func_02003f94(struct Orbit *object) {
  s32 z;

  object->angle += object->speed;
  object->x = IwramMulQ16(Math_Cosine(object->angle), object->radius);
  z = IwramMulQ16(Math_Sine(object->angle), object->radius);
  object->x += object->centreX;
  object->z = (z >> 1) + object->centreZ;
  if (object->growth != 141 && (gFrameCount & 1)) {
    object->radius = object->growth * Math_Sine(object->phase << 10);
    object->growth += 10;
  }
  if (object->speed == 0)
    return 0;
  if (object->growth == 141)
    object->speed -= 128;
  return 1;
}

s32 Func_02004038(struct Actor *object) {
  struct Actor *target = object->watchedObject;
  s32 steps;
  s32 from;

  object->bounce = (target->x - object->x) / (steps = 10);
  from = object->y;
  object->unk4c = (target->y - from + 0x100000) / steps;
  object->gravity = (target->z - object->z) / steps;
  BYTE_AT(object, 98) = steps;
  return 0;
}

s32 Func_02004084(struct Actor *object) {
  u8 left;

  object->x += object->bounce;
  object->y += object->unk4c;
  object->z += object->gravity;
  left = --BYTE_AT(object, 98);
  return left != 0;
}

#include "system/field_effect.h"

void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_020040b0(struct Actor *object) {
  s32 phase = gFrameCount & 7;

  if (phase == 0) {
    struct EffectOptions options;
    s32 position[3];

    options.palette = 7;
    options.type = 0x2a1;
    options.priority = 1;
    position[0] = object->x + (((Random16Far() & 31) - 16) << 16);
    position[1] = object->y + ((Random16Far() & 31) << 16) + 0x80000;
    position[2] = object->z;
    Func_020002f4(
        position[0], position[1], position[2] + 0x40000, 0, 0x8000, phase,
        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
}

struct SparkBytes {
  u8 unk00[0x62];
  u8 unk62;
  u8 unk63;
};

union SparkView {
  struct Actor actor;
  struct SparkBytes bytes;
};

extern const u8 gData_020060e0[], gData_02006094[], gData_020060b0[],
    gData_02006104[], gData_020060c0[], gData_02006128[], gData_02006158[];
void Func_02005ed4(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02005f0c(s32 a, s32 b);
s32 Func_02005f04(s32 a, s32 b);
s32 Func_02005f14(s32 frames);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Func_02005e94(s32 actor, s32 a);
void Func_02005f3c(void);

void Func_02004138(s32 idA, s32 idB) {
  struct Actor *a = Actor_GetById(idA);
  struct Actor *b = Actor_GetById(idB);
  s32 i;
  s16 *angle;
  s32 never;
  // Unused: gives the stack frame its 12 bytes.
  s32 unused[3];

  Func_02005f94(78);
  FieldEvent_Wait(30);
  Func_02005ed4(0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&a->x)[1] << 16, ((s16 *)&a->y)[1] << 16,
                             ((s16 *)&a->z)[1] << 16, 1);
  Func_02005f94(141);
  Call3(Func_02005dcc, 0x20000, 0x20000, 0x10000);
  Func_02005f0c(0x10000, 0);
  Func_02005f04(0x10003, 1);
  Func_02005f14(60);
  a->motionFlags = 0;
  a->acceleration = 0x3333;
  a->speedLimit = 0x3333;
  Actor_SetMovementTypeById(idA, gData_020060e0);
  Func_02005f94(194);
  FieldEvent_Wait(45);
  Func_02005e94(idA, 0x100);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
first:
  {
    struct Actor *spark = Import_ActorCreate(0x2a0, a->x, a->y, a->z);
    Func_02005f94(246);
    spark->x += ((Random16Far() & 31) - 16) << 16;
    spark->y += ((Random16Far() & 15) - 8) << 16;
    spark->speedLimit = 0x80000;
    spark->acceleration = 0x3333;
    spark->motionFlags = 0;
    Actor_SetPartAttribute(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Func_02005d5c(spark, 7);
    Actor_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Func_02005d6c(spark, gData_02006094);
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
  /* FAKEMATCH(dead): the never-true jump into the loop; as no test at all, 200
   * of 402 instructions differ in tla-en. */
  never = idB << 1;
  i = 0;
  if (never & 1) {
    i = 1;
    goto tail;
  }
second:
  {
    struct Actor *spark = (struct Actor *)Value4((s32 (*)())Import_ActorCreate,
                                                 0x2a0, b->x, b->y, b->z);
    Func_02005f94(195);
    angle = &spark->timer;
    *angle = (Random16Far() & 0x7fff) + 0x8000;
    spark->unk66 = ((i & 1) * 2 - 1) << ((Random16Far() & 3) + 9);
    spark->motionFlags = 0;
    spark->unk4c = 0x7a0000;
    spark->watchedObject = b;
    ((union SparkView *)spark)->bytes.unk62 = (16 - i) * 8 + 15;
    Actor_SetPartAttribute(spark, 2);
    Scene_SetActorSpritePriority(spark, 1);
    Func_02005d5c(spark, 7);
    Actor_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Call3(Vector_AddPolarOffsetFar, spark->unk4c, *angle, (s32)&spark->x);
    Func_02005d6c(spark, gData_020060b0);
    if (i == 3) {
      Call2(Func_02005e94, idB, 0x100);
      b->motionFlags = 0;
      b->acceleration = 0x3333;
      b->speedLimit = 0x3333;
      /* FAKEMATCH(barrier): the empty asm before the call; as no asm, 2 of 402
       * instructions differ in tla-en. */
      asm("");
      Actor_SetMovementTypeById(idB, gData_02006104);
    }
  tail:
    wait(8);
  }
  i++;
  if (i != 16)
    goto second;
  Func_02005f94(220);
  wait(16);
  i = 2;
third:
  {
    struct Actor *spark =
        (struct Actor *)Value4((s32 (*)())Import_ActorCreate, 0x2a0, b->x,
                               b->y + i * 65536 - 0x80000, b->z);
    spark->timer = Random16Far();
    spark->unk66 = 0x800;
    ((union SparkView *)spark)->bytes.unk62 = i;
    ((union SparkView *)spark)->bytes.unk63 = 1;
    spark->bounce = spark->x;
    spark->unk4c = 0;
    spark->gravity = spark->z;
    spark->watchedObject = b;
    spark->motionFlags = 0;
    spark->priorityFlags = 1;
    Actor_SetPartAttribute(spark, 2);
    Func_02005d5c(spark, 7);
    Actor_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Func_02005d6c(spark, gData_020060c0);
  }
  i += 2;
  if (i != 32)
    goto third;
  Func_02005f94(220);
  wait(50);
  Func_02005f04(0x7fff, 0);
  Func_02005f14(8);
  wait(16);
  Func_02005dcc(0, 0, 0);
  Func_02005e94(idA, 0);
  Func_02005e94(idB, 0);
  Func_02005f94(574);
  Func_02005f04(0x10000, 0);
  Func_02005f14(80);
  Actor_SetMovementTypeById(idA, gData_02006128);
  Actor_SetMovementTypeById(idB, gData_02006158);
  b->update = Func_020040b0;
  wait(120);
  b->update = 0;
  Func_02005f94(390);
  Func_02005f3c();
  wait(1);
}

#include "scene_tables.h"
#include "text/msg_ids.h"

void Actor_SetPositionAndReset(s32 actor, s32 x, s32 z);
void Actor_ArmCallback(s32 actor, s32 angle, s32 frames);
void Func_02004534(void);
#include "event_work.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)
#define EVENT_WORK ((struct EventWork *)Ram_HeapSlots->eventWork)

s32 Func_02005ea4();
s32 Func_02005e74();
s32 Func_02005eac();
s32 Func_02005e6c();
s32 Func_02005dd4();
s32 Party_LearnAbility();
void Actor_TurnToFaceEachOther(s32 actorId, s32 other, s32 frames);
void Actor_WaitForMoveById(s32 actorId);
void Actor_ResetAndSetPosition(u32 objectId, s32 x, s32 z);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

static inline void Call0(void (*f)()) {
  /* FAKEMATCH(wrapper): the call layer without arguments, through which the two
   * -1s before the call are negated in this order; as a direct call, 2 of 202
   * instructions differ in tla-en. */
  f();
}

/// Checking the tablet shows its message; the first time (flag 0x8d4 clear)
/// the scene with actor 6 follows, as Func_020030cc of Magma Rock does with
/// the Blaze tablet.
void Func_02004534(void) {
  struct Actor *object;

  Func_02005e14();
  Func_02005f2c(0);
  FieldMessage_Show(MsgCheckedTheTablet, 1);
  if (Value1(GameFlag_Test, 0x8d4) != 0) {
    Func_02005e1c();
  } else {
    Func_02005ea4((s32)MsgCheckedTheTablet);
    EVENT_WORK->messageId++;
    Call3(Actor_SetSpeedParameters, gMainRoutineWork.currentOwner, 0xb333,
          0x5999);
    Call3(Actor_SetSpeedParameters, 6, 0xb333, 0x5999);
    Func_02005e74(6, gMainRoutineWork.currentOwner);
    FieldEvent_Wait(1);
    Call3(Actor_ResetAndSetPositionInMode2, gMainRoutineWork.currentOwner, 256,
          136);
    Actor_ResetAndSetPositionInMode2(6, 272, 136);
    Actor_WaitForMoveById(gMainRoutineWork.currentOwner);
    Actor_WaitForMoveById(6);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_EAST, 0);
    Actor_ArmCallback(6, FACING_WEST, 0);
    FieldEvent_Wait(10);
    Call2(Func_02005eac, 6, 0);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(30);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 256, 152);
    Call3(Actor_ArmCallback, gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Actor_ArmCallback(6, FACING_SOUTH, 0);
    FieldEvent_Wait(20);
    Actor_SetModeById(6, 3);
    FieldEvent_Wait(30);
    Call3(Actor_ArmCallback, 6, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    Call3(Actor_SetPositionAndReset, 6, 264, 137);
    Actor_ArmCallback(6, FACING_NORTH, 0);
    FieldEvent_Wait(20);
    FieldMessage_Show(MsgCheckedTheTablet + 2, 1);
    Call3(Func_02005dcc, 0x50000, 0x70000, 0x10000);
    Call3(Func_02005dcc, -1, -1, 0xe666);
    /* FAKEMATCH(wrapper): the call through Call0's pointer, which negates the
     * two -1s before it in this order; as a direct call, 2 of 202 instructions
     * differ in tla-en. */
    Call0(Func_02005dd4);
    Call2(Func_02004138, 8, 6);
    Party_LearnAbility(6, 0x90);
    GameFlag_SetBit(0x8d4);
    Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, 6, 0);
    FieldEvent_Wait(10);
    Actor_SetModeById(6, 3);
    Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
    FieldEvent_Wait(20);
    Call3(Actor_SetSpeedParameters, 6, 0x13333, 0x9999);
    Actor_SetModeById(6, 2);
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (object != 0)
      Actor_ResetAndSetPosition(6, FIELD_AT(object, s16, 10),
                                FIELD_AT(object, s16, 18));
    Actor_WaitForMoveById(6);
    Func_02005e6c(6, 0, 0);
    Actor_SetActionCallback(Actor_GetById(8), 9);
    Func_02005e1c();
  }
}

void Func_02004740(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 107);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_020047b0(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 280, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_02004818(void) {
  if (GameFlag_Test(0x8d4)) {
    Func_02005e14();
    Func_02005f2c(0);
    FieldMessage_Show(MsgCheckedTheTablet, 1);
    Func_02005e1c();
  } else {
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 248, 136);
    Actor_SetPositionAndReset(gMainRoutineWork.currentOwner, 264, 136);
    Actor_ArmCallback(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
    FieldEvent_Wait(10);
    Func_02004534();
  }
}

void Func_02004880(void) {}

void Func_020007b4(void);

void Func_02004884(void) {
  Func_02005e14();
  Func_02005f2c(0);
  if (Actor_GetById(8)->x >> 19 == 65)
    GameFlag_SetBit(771);
  else
    GameFlag_ClearBit(771);
  Func_02005e1c();
}

void Func_020048bc(void) { Func_020007b4(); }

#include "graphics/vram_tab.h"
#include "system/dma.h"

extern u16 gData_0200621c[];
void Func_020006cc(struct Actor *object);

void Func_020048c4(void) {
  struct Actor *first = 0, *second = 0, *leader;
  struct FieldSprite *firstSprite, *secondSprite;
  struct VramBlockCacheEntry *entry;
  u16 *pair;
  u32 id, i;
  s32 busy;

  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  pair = gData_0200621c;
  if (*pair == 0xffff)
    goto found;
search:
  id = *pair;
  first = Actor_GetById(id);
  if (first->x >> 20 == leader->x >> 20 && first->z >> 20 == leader->z >> 20) {
    second = Actor_GetById(id - ((id & 1) << 1) + 1);
    goto found;
  }
  pair += 3;
  if (*pair != 0xffff)
    goto search;
found:
  busy = ((u8 *)first)[0x63] | ((u8 *)second)[0x63];
  if (busy != 0)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  firstSprite = first->sprite;
  secondSprite = second->sprite;
  second->motionFlags = busy;
  second->y += -0x1c0000;
  wait(1);
  entry = &gResourceTableEntries[secondSprite->vramBlock];
  second->watchedObject = (void *)(entry->offset + 0x6010000);
  entry = &gResourceTableEntries[firstSprite->vramBlock];
  first->watchedObject = (void *)(entry->offset + 0x6010000);
  Dma_Fill32(busy, second->watchedObject, entry->size);
  Dma_Set(first->watchedObject, second->watchedObject, 0x840000c0,
          (volatile u32 *)0x040000d4);
  first->watchedObject = (void *)((u8 *)first->watchedObject + 0x300);
  second->watchedObject = (void *)((u8 *)second->watchedObject + 0x300);
  leader->motionFlags = busy;
  i = 0;
step:
  if (i <= 4) {
    second->y += 0x53333;
    Dma_Set(first->watchedObject, second->watchedObject, 0x84000030,
            (volatile u32 *)0x040000d4);
    first->watchedObject = (void *)((u8 *)first->watchedObject + 0x100);
    second->watchedObject = (void *)((u8 *)second->watchedObject + 0x100);
  }
  leader->y += -0x38000;
  leader->terrainHeight = leader->y;
  first->scaleY += -0x1f00;
  wait(1);
  i++;
  if (i != 8)
    goto step;
  leader->motionFlags = 3;
  first->scaleY = 0x10000;
  second->y = second->terrainHeight;
  GameFlag_SetBit(first->timer);
  GameFlag_ClearBit(second->timer);
  Func_02005d5c(first, ((struct FieldSprite *)first->sprite)->frame + 1);
  Func_02005d5c(second, ((struct FieldSprite *)second->sprite)->frame - 1);
  Func_020006cc(first);
  Func_020006cc(second);
  FieldEvent_Wait(10);
  Func_02005e1c();
}
