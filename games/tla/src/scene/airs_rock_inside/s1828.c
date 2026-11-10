#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "call.h"
#include "system/field_effect.h"
#include "system/vector3.h"

extern char SceneId_Ov664Room4, SceneId_Ov664Room6, SceneId_Ov664Room7,
    SceneId_Ov664Room8, SceneId_Ov664Room9;
extern u32 gFrameCount;
extern const u8 gData_02005fd8[];
struct Actor *Actor_GetById(u32 id);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 skip);
void Func_02005f94(s32 cue);
void Func_02005d5c(struct Actor *object, s32 mode);
void Func_02005d6c(struct Actor *object, const u8 *script);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Func_02005d8c(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02005d94(struct Actor *object);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void FieldEvent_Wait(s32 frames);
void wait(s32 frames);
u32 Random16Far(void);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Func_02002bd0(void);
void Func_020017f4(void);
s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);
s32 Func_02000690(u32 layer, s32 x, s32 z);
void Func_02005ed4(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
s32 Func_02005dec(struct Actor *object);
void Func_020002f4(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, u32 flags,
                   struct EffectOptions *options);
void Func_02005d9c(s32 x, s32 z, s32 width, s32 height, s32 a, s32 b);
void Func_02005dac(s32 srcX, s32 srcZ, s32 width, s32 height, s32 destX,
                   s32 destZ);
void Func_02005d84(void);
void OverlayObject_WaitUntilIdle(struct Actor *object);
void Func_02005df4(s32 x, s32 z, s32 terrain, s32 height);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02001084(struct Actor *object, s32 a, s32 b);
void Func_02005d7c(struct Actor *object);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02005eec(s32 actor, s32 a);
void Func_02005ee4(void);

/// A cell of the map, and a step from one cell to the next.
struct Cell {
  s32 x, z;
};

struct EffectWork {
  u8 unknown[20];
  struct Actor *target;
};

#define TARGET(effect) ((effect)->target)

/// The gust picks up an object it passes over, to drop it at cell `at`
/// (x in the high half, z in the low).
#define PICK_UP(object, at)                                                    \
  do {                                                                         \
    (object)->watchedObject = gust;                                            \
    (object)->update = Func_020017f4;                                          \
    (object)->motionFlags = 0;                                                 \
    (object)->priorityFlags &= ~1;                                             \
    Actor_SetSingleChildFlags(object, 0);                                      \
    ((struct FieldSprite *)(object)->animation)->priority =                    \
        ((struct FieldSprite *)gust->animation)->priority;                     \
    gust->watchedObject = (object);                                            \
    carried = (at);                                                            \
  } while (0)

/// One shake of the walls (cue 157).
#define SHAKE()                                                                \
  (Func_02005f94(157), Call3(Func_02005dcc, 0x50000, 0x50000, 0x10000),        \
   Call3(Func_02005dcc, -1, -1, 0xe666))

void Func_02001828(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectWork *effect = (struct EffectWork *)Ram_HeapSlots->effectWork;
  struct Actor *gust, *spark, *object;
  struct Vector3 velocity;
  struct EffectOptions leaves, dust;
  struct Cell dir, cell;
  s32 i, steps = 0, carried;
  // Default: nothing reads this 0 before the loop sets hit (without it, 261
  // of 1527 instructions differ in tla-en).
  s32 hit = 0;

  /* FAKEMATCH(wrapper): calls forwarded through call.h's Call2, Call3, Call4
   * and Call6, so each builds its own constants; as direct calls, 416 of 1527
   * instructions differ in tla-en. */
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(207);
  gust =
      (struct Actor *)Import_ActorCreate(280, leader->x, leader->y, leader->z);
  Func_02005d5c(gust, 2);
  Actor_SetSingleChildFlags(gust, 0);
  gust->scaleX = 0x1b333;
  gust->scaleY = 0x3333;
  gust->motionFlags = 0;
  /* FAKEMATCH(signature): the target's word read as long, an alias set outside
   * struct Actor's, so the load need not wait for the motion flags' store; as
   * TARGET(effect), 2 of 1527 instructions differ in tla-en. */
  ((struct FieldSprite *)gust->animation)->priority =
      ((struct FieldSprite *)((struct Actor *)*(long *)&effect->target)
           ->animation)
          ->priority;
  Func_02005d8c(gust, TARGET(effect)->x, TARGET(effect)->y,
                TARGET(effect)->z + 0xffff0000);
  gust->terrainHeight = Map_GetTerrainHeight(0, gust->x, gust->z);
  while (gust->scaleY <= 0x14ccb) {
    gust->scaleY += 0x1999;
    FieldEvent_Wait(1);
  }
  Func_02005d94(gust);
  gust->z = TARGET(effect)->z + 0xffff0000;
  while (gust->scaleY <= 0x1cccb) {
    if ((gFrameCount & 3) == 0) {
      spark = (struct Actor *)Import_ActorCreate(
          0x2a0, TARGET(effect)->x, TARGET(effect)->y, TARGET(effect)->z);
      spark->priorityFlags |= 1;
      spark->motionFlags = 0;
      spark->terrainHeight = Map_GetTerrainHeight(0, spark->x, spark->z);
      spark->bounce = spark->x;
      spark->gravity = 0xcccc;
      spark->unk4c = spark->z;
      spark->speedLimit = Random16Far() & 0xffff;
      Func_02005d5c(spark, 7);
      Func_02005d6c(spark, gData_02005fd8);
      Actor_SetSingleChildFlags(spark, 0);
      Actor_SetPartAttribute(spark, 2);
      spark->update = Func_02002bd0;
    }
    if (gFrameCount & 1) {
      gust->scaleX += 655;
      gust->scaleY += 655;
      Actor_SetPartAttribute(TARGET(effect), 15);
    } else {
      Actor_SetPartAttribute(TARGET(effect), 4);
    }
    FieldEvent_Wait(1);
  }
  Actor_SetPartAttribute(TARGET(effect), 0);
  Func_02005f94(136);
  cell.x = TARGET(effect)->x >> 20;
  cell.z = TARGET(effect)->z >> 20;
  if (leader->x >> 20 < cell.x) {
    dir.x = 1;
    dir.z = 0;
  } else if (leader->x >> 20 > cell.x) {
    dir.x = -1;
    dir.z = 0;
  } else if (leader->z >> 20 < cell.z) {
    dir.x = 0;
    dir.z = 1;
  } else {
    dir.x = 0;
    dir.z = -1;
  }
  do {
    hit = Func_02000690(2, cell.x, cell.z);
    hit >>= 8;
    if (Map_GetTerrainHeight(2, cell.x << 20, cell.z << 20) !=
        gust->terrainHeight)
      hit = 0;
    cell.x += dir.x;
    cell.z += dir.z;
    steps += 2;
  } while (hit == 0);
  Func_02000690(2, cell.x, cell.z);
  {
    s32 x;
    s32 z;

    steps -= 2;
    x = (TARGET(effect)->x >> 16) + ((steps * dir.x) << 3);
    z = (TARGET(effect)->z >> 16) + ((steps * dir.z) << 3);
    ((struct FieldSprite *)gust->animation)->priority = 1;
    cell.x = x;
    cell.z = z;
    gust->speedLimit = 0x40000;
    gust->acceleration = 0x40000;
    Func_02005d8c(gust, cell.x << 16, gust->y, cell.z << 16);
    Call2((void (*)())Func_02005ed4, 0x50000, 0x50000);
    Call4((void (*)())Camera_MoveWithinMapBounds, x << 16, -1, z << 16, 1);
  }
  carried = 0;
  while (Func_02005dec(gust) == 0) {
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6) {
      object = Actor_GetById(8);
      if (gust->x >> 19 == object->x >> 19 && carried == 0)
        PICK_UP(object, 0x270025);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
      object = Actor_GetById(9);
      if (gust->x >> 20 == object->x >> 20 &&
          gust->z >> 20 == object->z >> 20 && carried == 0)
        PICK_UP(object, 0x290069);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
      object = Actor_GetById(10);
      if (gust->x >> 20 == object->x >> 20 &&
          gust->z >> 20 == object->z >> 20 && carried == 0)
        PICK_UP(object, 0xf0033);
    }
    if ((gFrameCount & 1) == 0) {
      leaves.priority = 1;
      leaves.type = 674;
      leaves.palette = 2;
      velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
      velocity.y = gust->y + ((Random16Far() & 31) << 16);
      velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
      Func_020002f4(gust->x, velocity.y, gust->z, velocity.x, 0, velocity.z,
                    EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                    &leaves);
    }
    FieldEvent_Wait(1);
  }
  if (carried != 0) {
    object = gust->watchedObject;
    object->x = (carried >> 16) << 19;
    object->z = carried << 19;
    object->velocityY = 0x80000;
    object->update = 0;
    object->motionFlags = 2;
    object->priorityFlags |= 1;
    object->priorityFlags &= ~6;
    Actor_SetSingleChildFlags(object, 1);
    Scene_SetActorSpritePriority(object, 2);
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6) {
      object->terrainHeight = 0xffe00000;
      Call6(Func_02005dac, 19, 16, 1, 1, 19, 18);
      GameFlag_SetBit(0x89f);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room8) {
      Call6(Func_02005dac, 19, 38, 1, 1, 20, 38);
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      Func_02005df4(0x1400000, 0x3400000, 0, 4);
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
      OverlayObject_WaitUntilIdle(object);
      wait(1);
      OverlayObject_WaitUntilIdle(object);
      Actor_SetSingleChildFlags(object, 0);
      object->motionFlags = 0;
      object->terrainHeight = 0;
      object->y = 0;
      Func_02005df4(0x780000, 0x1980000, 0, 4);
      if (GameFlag_Test(0x899))
        Func_02001084(object, 60, 0);
    }
  }
  if (hit == 255) {
    Func_02005f94(136);
  } else {
    cell.x = (gust->x >> 16) + dir.x * 48;
    cell.z = (gust->z >> 16) + dir.z * 48;
    gust->speedLimit = 0x8000;
    gust->acceleration = 0x8000;
    Call4((void (*)())Func_02005d8c, (s32)(gust), (s32)(cell.x << 16),
          (s32)(gust->y), (s32)(cell.z << 16));
    hit -= 30;
    i = 0;
    while (Func_02005dec(gust) == 0) {
      if (hit > 4)
        goto shrink;
      if (i == 0) {
        Func_02005f94(235);
        i = 1;
      }
      if ((gFrameCount & 1) == 0) {
        dust.priority = 1;
        {
          dust.startScaleX = dust.startScaleY =
              (Random16Far() & 0xffff) + 0x10000;
        }
        velocity.x = Math_Cosine((gFrameCount & 0x1ff) << 12) << 1;
        velocity.y = 0x10000;
        velocity.z = Math_Sine((gFrameCount & 0x1ff) << 12);
        Func_020002f4(gust->x, gust->y, gust->z, velocity.x, velocity.y,
                      velocity.z, EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY,
                      &dust);
      }
      if ((gFrameCount & 3) == 0) {
        if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4) {
          GameFlag_SetBit(0x300);
          cell.x = (gust->x >> 20) + 64;
          cell.z = gust->z >> 20;
          Func_02005d9c(90, 25, cell.x + 1, cell.z, 1, 3);
          Func_02005d9c(21, 88, 24, 89, 1, 2);
          Func_02005dac(23, 92, 2, 1, 23, 89);
          Func_02005dac(18, 33, 2, 1, 23, 25);
        } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room7) {
          cell.x = gust->x >> 20;
          cell.z = ((gust->z - gust->y) >> 20) + 64;
          if (hit == 0) {
            GameFlag_SetBit(772);
            Func_02005d9c(39, 82, cell.x, cell.z, 1, 1);
            Func_02005dac(39, 82, 1, 1, 38, 82);
            Call6(Func_02005dac, 38, 19, 1, 1, 38, 18);
          } else {
            GameFlag_SetBit(773);
            Call6(Func_02005d9c, 39, 82, cell.x, cell.z, 1, 1);
            Call6(Func_02005dac, 38, 98, 1, 1, 38, 99);
            Call6(Func_02005dac, 38, 35, 1, 1, 38, 37);
          }
        } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room9) {
          GameFlag_SetBit(774);
          cell.x = gust->x >> 20;
          Call6(Func_02005d9c, 15, 80, cell.x, 77, 1, 3);
          Call6(Func_02005dac, 15, 80, 1, 1, 15, 79);
          Call6(Func_02005dac, 17, 15, 2, 1, 15, 15);
        }
        Func_02005d84();
      }
      FieldEvent_Wait(1);
    }
    Func_02005f94(298);
  }
shrink:
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
  Func_02005d7c(gust);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room4) {
    if (hit == 5) {
      SHAKE();
      Call6(Func_02005d9c, 79, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      Call6(Func_02005d9c, 69, 0x40, 29, 85, 5, 7);
      Call6(Func_02005dac, 46, 20, 5, 5, 29, 21);
      Call6(Func_02005dac, 0x20, 89, 1, 1, 31, 89);
      Call6(Func_02005dac, 50, 86, 1, 1, 33, 87);
      GameFlag_SetBit(769);
    } else if (hit == 6) {
      SHAKE();
      Call6(Func_02005d9c, 74, 0x40, 29, 85, 5, 7);
      FieldEvent_Wait(30);
      SHAKE();
      Call6(Func_02005d9c, 0x40, 0x40, 29, 85, 5, 7);
      Call6(Func_02005dac, 52, 20, 5, 5, 29, 21);
      Call6(Func_02005dac, 33, 88, 1, 1, 33, 87);
      Call6(Func_02005dac, 0x30, 88, 1, 1, 31, 89);
      GameFlag_ClearBit(769);
    }
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov664Room6) {
    if (hit == 5) {
      SHAKE();
      Call6(Func_02005d9c, 79, 0x40, 0x20, 79, 5, 7);
      if (Actor_GetById(8)->x >> 19 == 65) {
        velocity.x = Actor_GetById(8)->x + 0xfff80000;
        velocity.y = Actor_GetById(8)->y;
        velocity.z = Actor_GetById(8)->z + 0x80000;
        Actor_SetSpritePriority(8, 2);
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          Func_020002f4(velocity.x + ((Random16Far() & 15) << 16),
                        velocity.y + ((Random16Far() & 31) << 16), velocity.z,
                        0, 0, -0x8000, EFFECT_USE_PRIORITY, &dust);
        }
        wait(30);
        SHAKE();
        Call6(Func_02005d9c, 0x40, 0x40, 0x20, 79, 5, 7);
        Actor_GetById(8)->priorityFlags |= 1;
      } else {
        FieldEvent_Wait(30);
        SHAKE();
        Call6(Func_02005d9c, 69, 0x40, 0x20, 79, 5, 7);
        Call6(Func_02005dac, 49, 15, 5, 5, 0x20, 15);
        Call6(Func_02005dac, 33, 82, 1, 1, 34, 82);
        Call6(Func_02005dac, 52, 81, 1, 1, 36, 81);
        GameFlag_ClearBit(770);
      }
    } else if (hit == 6) {
      SHAKE();
      if (Actor_GetById(8)->x >> 19 == 65) {
        velocity.x = Actor_GetById(8)->x + 0xfff80000;
        velocity.y = Actor_GetById(8)->y;
        velocity.z = Actor_GetById(8)->z;
        for (i = 0; i != 8; i++) {
          dust.priority = 1;
          Func_020002f4(velocity.x + ((Random16Far() & 15) << 16), velocity.y,
                        velocity.z - ((Random16Far() & 31) << 16), 0, 0,
                        -0x8000, EFFECT_USE_PRIORITY, &dust);
        }
        wait(20);
      } else {
        Call6(Func_02005d9c, 79, 0x40, 0x20, 79, 5, 7);
        Func_02005f94(157);
        FieldEvent_Wait(30);
        SHAKE();
        Call6(Func_02005d9c, 0x40, 0x40, 0x20, 79, 5, 7);
        Call6(Func_02005dac, 49, 21, 5, 5, 0x20, 15);
        Call6(Func_02005dac, 35, 81, 1, 1, 36, 81);
        Call6(Func_02005dac, 50, 82, 1, 1, 34, 82);
        GameFlag_SetBit(770);
      }
    }
  }
  FieldEvent_Wait(30);
  Func_02005eec(gMainRoutineWork.currentOwner, 1);
  Func_02005ee4();
  Func_02005e1c();
}
