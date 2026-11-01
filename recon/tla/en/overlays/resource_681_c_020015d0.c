// Matches: differ 0 (tla-en, 2026-11-01)
// A burst of power between two objects (a = idA, b = idB; Func_020019cc passes
// 12 and 4): sound 78, the camera centres on a, the screen zooms and fades, a
// takes its action script; sixteen sparks (object type 0x2a0) fly out of a while
// the blend flickers; sixteen more swirl round b, which takes its own script at
// the fourth; fifteen rise in a column over b; the screen fades back, both take
// their closing scripts and b runs Func_02001548 for 120 frames, then sound 390.
// Near copies: 664 02004138, 67c 02001620, 692 02002cd0.
// What closed it, in plain C: all three loops are goto loops; kind is one local
// for all three creates (an s32 constant, so the argument is rematerialized
// last and the 3-register loads use no r4, gap 21 to 9); struct Spark is one
// struct for every field (its bytes at 0x62 and 0x63 fall inside Actor.callback,
// and one alias set keeps the stores of a spark in order, differ 12 to 2);
// unused[3] is the native's unread frame.
// What the barriers are for: a > i > b > idA > idB is the native's allocation
// order (r7, r8, r10, r9, r11), and it comes out with every reference counted
// once, loops unweighted: a 10, i 17, b 14, idA 6, idB 6 by floor_log2 * refs /
// live. This compiler weights goto loops by CFG depth, which ranks b first and
// idB over idA, so the asm inputs add references to a, i and idA and nothing
// else. No C shape tried changes the weights.
#include "types.h"
#include "actor.h"
#include "call.h"
#include "system/io_reg.h"

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
struct Actor *Actor_GetById(u32 id);
void Func_02005014(s32 cue);
void FieldEvent_Wait(s32 frames);
void Func_02004f44(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02004e24(s32 a, s32 b, s32 c);
void Func_02004f9c(s32 a, s32 b);
s32 Func_02004f94(s32 a, s32 b);
s32 Func_02004fa4(s32 frames);
void Actor_SetMovementTypeById(s32 actor, const void *callback);
void Func_02004efc(s32 actor, s32 a);
void *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Actor_SetPartAttribute(void *object, s32 value);
void Scene_SetActorSpritePriority(void *object, s32 priority);
void Func_02004d9c(void *object, s32 mode);
void Actor_SetSingleChildFlags(void *object,
                                        u32 value);
void Animation_SetStateFlags(void *object, s32 flags);
void Func_02004dac(void *object, const u8 *script);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
void wait(s32 frames);
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

  Func_02005014(78);
  FieldEvent_Wait(30);
  Func_02004f44(0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&a->x)[1] << 16, ((s16 *)&a->y)[1] << 16,
                             ((s16 *)&a->z)[1] << 16, 1);
  Func_02005014(141);
  /* FAKEMATCH(wrapper): forwarded, so each argument builds its own 0x20000; as a
   * direct call, 17 of 402 instructions differ in tla-en. */
  Call3(Func_02004e24, 0x20000, 0x20000, 0x10000);
  Func_02004f9c(0x10000, 0);
  Func_02004f94(0x10003, 1);
  Func_02004fa4(60);
  a->motionFlags = 0;
  a->acceleration = 0x3333;
  a->speedLimit = 0x3333;
  Actor_SetMovementTypeById(idA, gData_02005264);
  Func_02005014(194);
  FieldEvent_Wait(45);
  /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2 of
   * 402 instructions differ in tla-en. */
  Call2(Func_02004efc, idA, 0x100);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  /* FAKEMATCH(barrier): holds i = 0 below the blend store; as no asm, 2 of 402
   * instructions differ in tla-en. */
  asm("");
  i = 0;
first:
  /* FAKEMATCH(barrier): two references each for a and i, ranking a over i over
   * b; as no asm, 175 of 402 instructions differ in tla-en. */
  asm("" : : "r"(a), "r"(i));
  {
    struct Spark *spark = Import_ActorCreate(kind, a->x, a->y, a->z);
    Func_02005014(246);
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
    struct Spark *spark = Import_ActorCreate(kind, b->x, b->y, b->z);
    Func_02005014(195);
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
    /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2 of
     * 402 instructions differ in tla-en. */
    Call3(Vector_AddPolarOffsetFar, spark->unk4c, *angle, (s32)&spark->x);
    Func_02004dac(spark, gData_02005234);
    if (i == 3) {
      /* FAKEMATCH(wrapper): forwarded for its argument order; as a direct call, 2
       * of 402 instructions differ in tla-en. */
      Call2(Func_02004efc, idB, 0x100);
      b->motionFlags = 0;
      b->acceleration = 0x3333;
      b->speedLimit = 0x3333;
      /* FAKEMATCH(barrier): references to idA rank it over idB and hold idB in r0
       * before the script load; as no asm, 22 of 402 instructions differ in
       * tla-en. */
      asm("" : : "r"(idA));
      Actor_SetMovementTypeById(idB, gData_02005288);
    }
    wait(8);
  }
  i++;
  if (i != 16)
    goto second;
  Func_02005014(220);
  wait(16);
  i = 2;
third:
  {
    struct Spark *spark = Import_ActorCreate(kind, b->x, (i << 16) + b->y - 0x80000, b->z);
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
  Func_02005014(220);
  wait(50);
  Func_02004f94(0x7fff, 0);
  Func_02004fa4(8);
  wait(16);
  Func_02004e24(0, 0, 0);
  Func_02004efc(idA, 0);
  Func_02004efc(idB, 0);
  Func_02005014(574);
  Func_02004f94(0x10000, 0);
  Func_02004fa4(80);
  Actor_SetMovementTypeById(idA, gData_020052ac);
  Actor_SetMovementTypeById(idB, gData_020052dc);
  b->update = Func_02001548;
  wait(120);
  b->update = 0;
  Func_02005014(390);
  Func_02004fd4();
  wait(1);
}
