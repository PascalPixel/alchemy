#include "types.h"
#include "actor.h"
#include "call.h"

struct SparkBytes {
  u8 unk00[0x62];
  u8 unk62;
  u8 unk63;
};

union SparkView {
  struct Actor actor;
  struct SparkBytes bytes;
};

#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

extern const u8 gData_020060e0[], gData_02006094[], gData_020060b0[],
    gData_02006104[], gData_020060c0[], gData_02006128[], gData_02006158[];
struct Actor *Actor_GetById(u32 id);
void Func_02005f94(s32 cue);
void FieldEvent_Wait(s32 frames);
void Func_02005ed4(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02005f0c(s32 a, s32 b);
s32 Func_02005f04(s32 a, s32 b);
s32 Func_02005f14(s32 frames);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Func_02005e94(s32 actor, s32 a);
struct Actor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02005d5c(struct Actor *object, s32 mode);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Func_02005d6c(struct Actor *object, const u8 *script);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
void wait(s32 frames);
void Func_020040b0(void);
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
  a->acceleration = SPLIT_CONSTANT(0x3300, 51);
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
    spark->acceleration = SPLIT_CONSTANT(0x3300, 51);
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
    angle = &spark->action;
    *angle = (Random16Far() & SPLIT_CONSTANT(0x7f00, 255)) + 0x8000;
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
      b->acceleration = SPLIT_CONSTANT(0x3300, 51);
      b->speedLimit = SPLIT_CONSTANT(0x3300, 51);
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
    spark->action = Random16Far();
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
  Func_02005f94(SPLIT_CONSTANT(0x200, 62));
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
