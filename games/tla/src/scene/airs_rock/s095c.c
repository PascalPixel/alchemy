/// @file
/// The Whirlwind gust in Air's Rock.

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "event_inline.h"

extern s32 gFrameCount;
extern s16 gData_02004674[][4];
extern const u8 gData_02003d1c[];
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003b28(struct Actor *object, s32 mode);
void Func_02003b38(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
u32 Random16Far(void);
void Func_02000840(void);
s32 Math_Cosine(s32 angle);
s32 Func_02000348(u32 layer, s32 x, s32 z);
s32 Func_02003ba8(struct Actor *object);
void Func_02003b50(void);
void Func_02003b48(struct Actor *object);
void Func_02003c38(s32 actor, s32 a);
void Func_02003c30(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

struct EffectWork {
  u8 unknown[20];
  struct Actor *target;
};

/// The actor word at 84 seen as bit fields, for one store (see Func_0200095c).
struct MotionView {
  u8 pad[84];
  u32 kind : 8;
  u32 flags : 8;
};

/// A map cell, or a step between cells, kept as one 8-byte pair: written by the
/// word, it keeps the native's store before the travel loop.
struct Cell {
  s32 x, z;
};

/// Whirlwind on a target in Air's Rock: a gust (object type 280) rises from the
/// target and grows, shedding sparks every fourth frame while the target
/// flickers; then it travels in the leader's facing, two cells a step, until
/// Func_02000348 finds a cell with something on it, throwing leaves as it goes.
/// With nothing there (255) it ends with cue 136; otherwise it stops one and a
/// half cells short and blows the thing found along, copying its cells each
/// fourth frame, and sets that thing's flag (0x850 + hit - 1) with cue 298.
/// Then the gust shrinks and rises away over 8 frames.
void Func_0200095c(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectWork *effect = (struct EffectWork *)Ram_HeapSlots->effectWork;
  struct Actor *gust, *spark;
  struct Vector3 velocity;
  struct EffectOptions leaves, dust;
  struct Cell delta, cell;
  s32 cellZ, cellX, newZ, newX, flag = 0, steps = 0, hit, angle, i;

  /* FAKEMATCH(wrapper): calls forwarded through call.h's Value4, Call2, Call4
   * and Call6, so each builds its own constants; as direct calls, 382 of 632
   * instructions differ in tla-en. */
  Func_02003bc8();
  Event_ResolvePendingActions(0);
  Sound_Play(207);
  gust = FieldActor_Create(280, leader->x, leader->y, leader->z);
  Func_02003b28(gust, 2);
  Actor_SetSingleChildFlags(gust, 0);
  gust->scaleX = 0x1b333;
  gust->scaleY = 0x3333;
  // Stored through a bit field view: its alias set frees the load after it, as
  // the native orders it (as gust->motionFlags, 2 of 632 differ in tla-en).
  ((struct MotionView *)gust)->flags = flag;
  ((struct FieldSprite *)gust->animation)->priority =
      ((struct FieldSprite *)effect->target->animation)->priority;
  Actor_SetMoveTarget(gust, effect->target->x, effect->target->y,
                      effect->target->z + 0xffff0000);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += 0x1999;
    Event_Wait(1);
  }
  Script_WaitForEventTimeout(gust);
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
      // 655, not 455: 200 << 1 + 255 in the listing.
      gust->scaleX += 655;
      gust->scaleY += 655;
      Actor_SetPartAttribute(effect->target, 15);
    } else {
      Actor_SetPartAttribute(effect->target, 4);
    }
    Event_Wait(1);
  }
  Actor_SetPartAttribute(effect->target, 0);
  Sound_Play(136);
  cell.x = effect->target->x >> 20;
  cell.z = effect->target->z >> 20;
  angle = (leader->angle + 0x2000) & 0xc000;
  delta.x = Math_Cosine(angle) >> 16;
  delta.z = Math_Sine(angle) >> 16;
  do {
    hit = Func_02000348(2, cell.x, cell.z);
    hit >>= 8;
    cell.x += delta.x;
    cell.z += delta.z;
    steps += 2;
  } while (hit == 0);
  flag = Func_02000348(2, cell.x, cell.z);
  flag >>= 8;
  flag -= 1;
  steps -= 2;
  newX = ((s16 *)&effect->target->x)[1] + ((steps * delta.x) << 3);
  newZ = ((s16 *)&effect->target->z)[1] + ((steps * delta.z) << 3);
  ((struct FieldSprite *)gust->animation)->priority = 1;
  cell.x = newX;
  cell.z = newZ;
  gust->speedLimit = 0x40000;
  gust->acceleration = 0x40000;
  Actor_SetMoveTarget(gust, cell.x << 16, gust->y, cell.z << 16);
  FieldCamera_SetSpeed(0x50000, 0x50000);
  FieldCamera_MoveTo((s32)(newX << 16), (s32)(-1), (s32)(newZ << 16), (s32)(1));
  while (Func_02003ba8(gust) == 0) {
    if ((gFrameCount & 1) == 0) {
      leaves.priority = 1;
      leaves.type = 674;
      leaves.palette = 2;
      velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = gust->y + ((Random16Far() & 31) << 16);
      velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
      Func_0200015c(gust->x, velocity.y, gust->z, velocity.x, 0, velocity.z,
                    EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                    &leaves);
    }
    Event_Wait(1);
  }
  if (hit == 255) {
    Sound_Play(136);
  } else {
    cell.x = ((s16 *)&gust->x)[1] + delta.x * 48;
    cell.z = ((s16 *)&gust->z)[1] + delta.z * 48;
    gust->speedLimit = 0x8000;
    gust->acceleration = 0x8000;
    Actor_SetMoveTarget(gust, cell.x << 16, gust->y, cell.z << 16);
    Sound_Play(235);
    hit -= 30;
    while (Func_02003ba8(gust) == 0) {
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        dust.startScaleX = dust.startScaleY =
            (Random16Far() & 0xffff) + 0x10000;
        velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = 0x10000;
        velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
        Func_0200015c(gust->x, gust->y, gust->z, velocity.x, velocity.y,
                      velocity.z, EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                      &dust);
      }
      if ((gFrameCount & 3) == 0) {
        s32 sourceX;
        cell.x = gData_02004674[hit][2] + (gust->x >> 20);
        cell.z = gData_02004674[hit][3] + (gust->z >> 20);
        cellX = cell.x;
        cellZ = cell.z;
        if (hit & 1) {
          sourceX = (u16)gData_02004674[hit][0];
          MapGrid_CopyCellsTo((s16)sourceX, gData_02004674[hit][1], cellX,
                              cellZ - 1, 1, 3);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 2, cellX,
                                     cellZ);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 2,
                                     gust->x >> 20, gust->z >> 20);
        } else {
          sourceX = (u16)gData_02004674[hit][0];
          MapGrid_CopyCellsTo((s16)sourceX, gData_02004674[hit][1], cellX,
                              cell.z - (hit >> 1), 1, 1);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 1, cellX,
                                     cellZ);
          MapGrid_CopyCellAttributes(gData_02004674[hit][0],
                                     gData_02004674[hit][1], 1, 1,
                                     gust->x >> 20, gust->z >> 20);
        }
        Func_02003b50();
      }
      Event_Wait(1);
    }
    if (hit == 2) {
      MapGrid_CopyCellsTo(gData_02004674[2][0], gData_02004674[2][1], cell.x,
                          cell.z, 1, 1);
      Func_02003b50();
    }
    Sound_Play(298);
    Event_SetFlag(flag + 0x850);
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
    Event_Wait(1);
  }
  Func_02003b48(gust);
  Event_Wait(30);
  Func_02003c38(gMainRoutineWork.currentOwner, 1);
  Func_02003c30();
  Func_02003bd0();
}
