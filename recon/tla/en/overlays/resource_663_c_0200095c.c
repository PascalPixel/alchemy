// Draft, differ 491 of 632, skeleton gap 109 (tla-en, 2026-11-01)
// Whirlwind in Air's Rock: a gust (object type 280)
// rises from the target (the effect work's object at 0x14) and grows,
// shedding sparks (object type 0x2a0, Func_02000840) every fourth frame
// while the target flickers; then it travels in the leader's facing, two
// cells a step, until Func_02000348 finds a cell with something on it,
// throwing leaves (Effect_Spawn, object type 674) as it goes. Nothing
// there (0xff) ends with cue 136; otherwise it stops one and a half cells
// short and blows the thing found (gData_02004674 row hit - 30: its cells'
// source and offset), copying its cells and attributes along each fourth
// frame while it throws dust, sets that thing's flag (0x850 + hit - 1)
// with cue 298; then the gust shrinks and rises away over 8 frames.
// Left: allocation, and it needs lighter refs than alchemy-gcc gives. The
// native's frame is 128 and it saves nothing around a call in the blow loop:
// the cell copies sit in r10 and r9, 0x10000 in r10 across Math_Cosine, 3 in r4.
// Those refs are 6, 5 and 3 unweighted, none above 4 * calls crossed, so r4 and
// r2 are not profitable; at flow's weight of 2 for a loop they are, which gives
// cellX r4 with a save at sp+16, 0x10000 r2 with a save at sp+20, a frame of 136
// and a rotated reload set for the whole function. A second entry into the loop
// (so it is no natural loop) gives the frame of 128 at once: `if ((hit & 3) >= 0)
// goto blowTest;`, always true, folded by combine to the listing's plain `b`.
// With the same on travel it reached differ 389, skeleton gap 47; it is dead code
// on every loop, not source, and left out. The native looks as if goto loops had
// no weight: that is a compiler question (K3), not settled here.
// Also left: the native computes the [hit][1] offset (r8) before the branch on
// hit & 1. An unread `sourceY = (u16)gData_02004674[hit][1];` beside sourceX
// gives it and differ 422, skeleton gap 57, but is a dead device short of 0.
// Found: x and z (sp+20), dx and dz (sp+28) are 8-byte structs. A DImode pseudo
// written by the word keeps the native's otherwise dead store before the travel
// loop, and the copies in the blow loop survive CSE. The target is read at each
// use, never a variable. A folded jump does not reset loop depth: find_basic_blocks
// reuses the bb notes. Named struct table fields made the skeleton worse (67).
// Kept: the travel and blow loops are built from labels, because as while
// loops loop.c hoists the addresses of dust and velocity out of them. The table
// is not const: every call makes the native read it again. A local holding the
// column zero-extended gives the native's ldrh with the shifts in each arm.
// Call2 and Call6 stop constants being shared between arguments.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "call.h"

extern s32 gFrameCount;
extern s16 gData_02004674[][4];
extern const u8 gData_02003d1c[];
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003c88(s32 skip);
void Func_02003cd8(s32 cue);
void Func_02003b28(struct Actor *object, s32 mode);
void Func_02003b38(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Func_02003b58(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003b60(struct Actor *object);
void FieldEvent_Wait(s32 frames);
u32 Random16Far(void);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Func_02000840(void);
s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);
s32 Func_02000348(u32 layer, s32 x, s32 z);
void Func_02003c20(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Func_02003ba8(struct Actor *object);
void Func_02003b68(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02003b80(s32 x, s32 z, s32 a, s32 b, s32 width, s32 height);
void Func_02003b50(void);
void GameFlag_SetBit(s32 flag);
void Func_02003b48(struct Actor *object);
void Func_02003c38(s32 actor, s32 a);
void Func_02003c30(void);

struct EffectWork {
  u8 unknown[20];
  struct Actor *target;
};

struct Cell {
  s32 x, z;
};

void Func_0200095c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectWork *effect = (struct EffectWork *)Ram_HeapSlots->effectWork;
  struct Actor *gust, *spark;
  struct Vector3 velocity;
  struct EffectOptions leaves, dust;
  struct Cell delta, cell;
  s32 cellZ, cellX, newZ, newX, flag = 0, steps = 0, hit, angle, i;

  Func_02003bc8();
  Func_02003c88(0);
  Func_02003cd8(207);
  gust =
      (struct Actor *)Import_ActorCreate(280, leader->x, leader->y, leader->z);
  Func_02003b28(gust, 2);
  Actor_SetSingleChildFlags(gust, 0);
  gust->scaleX = 0x1b333;
  gust->scaleY = 0x3333;
  gust->motionFlags = (u8)flag;
  ((struct FieldSprite *)gust->animation)->priority =
      ((struct FieldSprite *)effect->target->animation)->priority;
  Func_02003b58(gust, effect->target->x, effect->target->y,
                effect->target->z + 0xffff0000);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += 0x1999;
    FieldEvent_Wait(1);
  }
  Func_02003b60(gust);
  gust->z = effect->target->z + 0xffff0000;
  while (gust->scaleY <= 0x1cccb) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(0x2a0, effect->target->x, 0,
                                                 effect->target->z);
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->bounce = spark->x;
      spark->gravity = 0xcccc;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02003b28(spark, 7);
      Func_02003b38(spark, gData_02003d1c);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_02000840;
    }
    if (gFrameCount & 1) {
      gust->scaleX += 455;
      gust->scaleY += 455;
      Actor_SetPartAttribute(effect->target, 15);
    } else {
      Actor_SetPartAttribute(effect->target, 4);
    }
    FieldEvent_Wait(1);
  }
  Actor_SetPartAttribute(effect->target, 0);
  Func_02003cd8(136);
  cell.x = effect->target->x >> 20;
  cell.z = effect->target->z >> 20;
  angle = (leader->angle + 0x2000) & 0xc000;
  delta.x = Math_Cosine(angle) >> 16;
  delta.z = Math_Sine(angle) >> 16;
  do {
    hit = Func_02000348(2, cell.x, cell.z);
    cell.x += delta.x;
    cell.z += delta.z;
    hit >>= 8;
    steps += 2;
  } while (hit == 0);
  flag = (Func_02000348(2, cell.x, cell.z) >> 8) - 1;
  steps -= 2;
  newX = ((s16 *)&effect->target->x)[1] + ((steps * delta.x) << 3);
  newZ = ((s16 *)&effect->target->z)[1] + ((steps * delta.z) << 3);
  ((struct FieldSprite *)gust->animation)->priority = 1;
  gust->speedLimit = 0x40000;
  gust->acceleration = 0x40000;
  cell.x = newX;
  cell.z = newZ;
  Func_02003b58(gust, cell.x << 16, gust->y, cell.z << 16);
  Call2(Func_02003c20, 0x50000, 0x50000);
  Camera_MoveWithinMapBounds(newX << 16, -1, newZ << 16, 1);
  goto travelTest;
travel:
  if ((gFrameCount & 1) == 0) {
    leaves.priority = 1;
    leaves.type = 674;
    leaves.palette = 2;
    velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
    velocity.y = gust->y + ((Random16Far() & 31) << 16);
    velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
    Effect_Spawn(gust->x, velocity.y, gust->z, velocity.x, 0, velocity.z,
                 EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                 &leaves);
  }
  FieldEvent_Wait(1);
travelTest:
  if (Func_02003ba8(gust) == 0)
    goto travel;
  if (hit == 255) {
    Func_02003cd8(136);
  } else {
    cell.x = ((s16 *)&gust->x)[1] + delta.x * 48;
    cell.z = ((s16 *)&gust->z)[1] + delta.z * 48;
    gust->speedLimit = 0x8000;
    gust->acceleration = 0x8000;
    Func_02003b58(gust, cell.x << 16, gust->y, cell.z << 16);
    Func_02003cd8(235);
    hit -= 30;
    goto blowTest;
  blow:
    if ((gFrameCount & 1) == 0) {
      dust.priority = 1;
      dust.startScaleX = dust.startScaleY = (Random16Far() & 0xffff) + 0x10000;
      velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = 0x10000;
      velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
      Effect_Spawn(gust->x, gust->y, gust->z, velocity.x, velocity.y,
                   velocity.z, EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                   &dust);
    }
    if ((gFrameCount & 3) == 0) {
      s32 sourceX;
      cell.x = gData_02004674[hit][2] + (gust->x >> 20);
      cell.z = gData_02004674[hit][3] + (gust->z >> 20);
      cellX = cell.x;
      cellZ = cell.z;
      sourceX = (u16)gData_02004674[hit][0];
      if (hit & 1) {
        Call6(Func_02003b68, (s16)sourceX, gData_02004674[hit][1], cellX,
              cellZ - 1, 1, 3);
        Call6(Func_02003b80, gData_02004674[hit][0], gData_02004674[hit][1], 1,
              2, cellX, cellZ);
        Call6(Func_02003b80, gData_02004674[hit][0], gData_02004674[hit][1], 1,
              2, gust->x >> 20, gust->z >> 20);
      } else {
        Call6(Func_02003b68, (s16)sourceX, gData_02004674[hit][1], cellX,
              cell.z - (hit >> 1), 1, 1);
        Call6(Func_02003b80, gData_02004674[hit][0], gData_02004674[hit][1], 1,
              1, cellX, cellZ);
        Call6(Func_02003b80, gData_02004674[hit][0], gData_02004674[hit][1], 1,
              1, gust->x >> 20, gust->z >> 20);
      }
      Func_02003b50();
    }
    FieldEvent_Wait(1);
  blowTest:
    if (Func_02003ba8(gust) == 0)
      goto blow;
    if (hit == 2) {
      Call6(Func_02003b68, gData_02004674[2][0], gData_02004674[2][1], cell.x,
            cell.z, 1, 1);
      Func_02003b50();
    }
    Func_02003cd8(298);
    GameFlag_SetBit(flag + 0x850);
  }
  gust->acceleration = 0;
  gust->speedLimit = 0;
  gust->targetZ = 0;
  gust->targetY = 0;
  gust->targetX = 0;
  for (i = 0; i != 8; i++) {
    gust->scaleX -= 0x2666;
    gust->scaleY -= 0x4000;
    gust->y += 0x38000;
    FieldEvent_Wait(1);
  }
  Func_02003b48(gust);
  FieldEvent_Wait(30);
  Func_02003c38(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  Func_02003bd0();
}
