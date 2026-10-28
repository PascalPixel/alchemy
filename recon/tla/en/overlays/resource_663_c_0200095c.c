// Draft, differ 581 of 632: Whirlwind in Air's Rock: a gust (object type 280)
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
// Left: a rough reading: the native keeps the gust in r7, the effect work in
// r8, the leader in r10 and the hit in r11, the cell, its step and the flag on
// the stack (sp+16 to sp+32), the two spawn options at sp+36 and sp+76 and the
// velocity at sp+116; most of the difference is register choice and the order
// of the spawn's stores.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "system/vector3.h"
#include "call.h"

extern s32 gFrameCount;
extern const s16 gData_02004674[][4];
extern const u8 gData_02003d1c[];
void Func_02003bc8(void);
void Func_02003bd0(void);
void Func_02003c88(s32 skip);
void Func_02003cd8(s32 cue);
void Func_02003b28(struct Actor *object, s32 mode);
void Func_02003b38(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        u32 value);
void Func_02003b58(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02003b60(struct Actor *object);
void FieldEvent_WaitFrames(s32 frames);
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

void Func_0200095c(void) {
  struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
  u8 *effect = (u8 *)Ram_HeapSlots->effectWork;
  struct Actor *gust, *spark, *target;
  struct EffectOptions dust, leaves;
  struct Vector3 velocity;
  s32 flag = 0, steps = 0, x, z, dx, dz, hit, angle, i;
  const s16 *row;

  Func_02003bc8();
  Func_02003c88(0);
  Func_02003cd8(207);
  gust = (struct Actor *)Import_ActorCreate(280, leader->x, leader->y,
                                                     leader->z);
  Func_02003b28(gust, 2);
  Actor_SetSingleChildFlags(gust, 0);
  gust->scaleX = 0x1b333;
  gust->scaleY = 0x3333;
  gust->motionFlags = (u8)flag;
  target = *(struct Actor **)(effect + 20);
  ((struct FieldSprite *)gust->animation)->priority =
      ((struct FieldSprite *)target->animation)->priority;
  Func_02003b58(gust, target->x, target->y, target->z + 0xffff0000);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += 0x1999;
    FieldEvent_WaitFrames(1);
  }
  Func_02003b60(gust);
  gust->z = (*(struct Actor **)(effect + 20))->z + 0xffff0000;
  while (gust->scaleY <= 0x1cccb) {
    if ((gFrameCount & 3) == 0) {
      target = *(struct Actor **)(effect + 20);
      spark = (struct Actor *)Import_ActorCreate(0x2a0, target->x, 0,
                                                          target->z);
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
      Actor_SetPartAttribute(*(struct Actor **)(effect + 20), 15);
    } else {
      Actor_SetPartAttribute(*(struct Actor **)(effect + 20), 4);
    }
    FieldEvent_WaitFrames(1);
  }
  Actor_SetPartAttribute(*(struct Actor **)(effect + 20), 0);
  Func_02003cd8(136);
  target = *(struct Actor **)(effect + 20);
  x = target->x >> 20;
  z = target->z >> 20;
  angle = (leader->angle + 0x2000) & 0xc000;
  dx = Math_Cosine(angle) >> 16;
  dz = Math_Sine(angle) >> 16;
  do {
    hit = Func_02000348(2, x, z) >> 8;
    x += dx;
    z += dz;
    steps += 2;
  } while (hit == 0);
  flag = (Func_02000348(2, x, z) >> 8) - 1;
  steps -= 2;
  target = *(struct Actor **)(effect + 20);
  x = ((s16 *)&target->x)[1] + ((steps * dx) << 3);
  z = ((s16 *)&target->z)[1] + ((steps * dz) << 3);
  ((struct FieldSprite *)gust->animation)->priority = 1;
  gust->speedLimit = 0x40000;
  gust->acceleration = 0x40000;
  Func_02003b58(gust, x << 16, gust->y, z << 16);
  Func_02003c20(0x50000, 0x50000);
  Camera_MoveWithinMapBounds(x << 16, -1, z << 16, 1);
  while (Func_02003ba8(gust) == 0) {
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
    FieldEvent_WaitFrames(1);
  }
  if (hit == 255) {
    Func_02003cd8(136);
  } else {
    x = ((s16 *)&gust->x)[1] + dx * 48;
    z = ((s16 *)&gust->z)[1] + dz * 48;
    gust->speedLimit = 0x8000;
    gust->acceleration = 0x8000;
    Func_02003b58(gust, x << 16, gust->y, z << 16);
    Func_02003cd8(235);
    hit -= 30;
    while (Func_02003ba8(gust) == 0) {
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        dust.startScaleX = dust.startScaleY =
            (Random16Far() & 0xffff) + 0x10000;
        velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = 0x10000;
        velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
        Effect_Spawn(gust->x, gust->y, gust->z, velocity.x, velocity.y,
                     velocity.z, EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                     &dust);
      }
      if ((gFrameCount & 3) == 0) {
        row = gData_02004674[hit];
        x = row[2] + (gust->x >> 20);
        z = row[3] + (gust->z >> 20);
        if (hit & 1) {
          Call6(Func_02003b68, row[0], row[1], x, z - 1, 1, 3);
          Call6(Func_02003b80, row[0], row[1], 1, 2, x, z);
          Call6(Func_02003b80, row[0], row[1], 1, 2, gust->x >> 20,
                gust->z >> 20);
        } else {
          Call6(Func_02003b68, row[0], row[1], x, z - (hit >> 1), 1, 1);
          Call6(Func_02003b80, row[0], row[1], 1, 1, x, z);
          Call6(Func_02003b80, row[0], row[1], 1, 1, gust->x >> 20,
                gust->z >> 20);
        }
        Func_02003b50();
      }
      FieldEvent_WaitFrames(1);
    }
    if (hit == 2) {
      Call6(Func_02003b68, gData_02004674[2][0], gData_02004674[2][1], x, z, 1,
            1);
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
    FieldEvent_WaitFrames(1);
  }
  Func_02003b48(gust);
  FieldEvent_WaitFrames(30);
  Func_02003c38(gAdventure.currentOwner, 1);
  Func_02003c30();
  Func_02003bd0();
}
