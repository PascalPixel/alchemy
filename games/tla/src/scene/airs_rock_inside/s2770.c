/// @file
/// Scene code of overlay 0x664, in ROM order, from s2770's offset up to the next object the overlay links.

#include "airs_rock_inside.h"
#include "system.h"
#include "task.h"

void Func_020026c0(void);

void Func_02002770(void) {
  s32 value = -1;

  gData_02006c0c = value;
  {
    /* FAKEMATCH(temporary): the priority in a local, which keeps the constant
     * ahead of the task function's load; as a literal argument, 2 of 10
     * instructions differ in tla-en. */
    s32 priority = 0x480;
    Task_Add(Func_020026c0, priority);
  }
}

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
extern char SceneId_Ov664Room1, SceneId_Ov664Room2, SceneId_Ov664Room3,
    SceneId_Ov664Room5, SceneId_Ov664Room6, SceneId_Ov664Room7,
    SceneId_Ov664Room8, SceneId_Ov664Room9;

void Func_02002790(void) {
  struct Actor *actor = Actor_GetById(Func_02005f84());
  s32 x = actor->x >> 20;
  s32 y = actor->y >> 20;
  s32 z = actor->z >> 20;
  s32 cell = (x << 16) + z;
  s32 scene;

  if (cell == gData_02006c0c)
    return;
  gData_02006c0c = cell;
  gData_020073dc = 1;
  gData_020073e0 = 2;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov664Room1 || scene == (s32)&SceneId_Ov664Room2 ||
      scene == (s32)&SceneId_Ov664Room3) {
    gData_020073e4 = x;
    gData_020073e8 = z - y + 64;
  } else if (scene == (s32)&SceneId_Ov664Room5 ||
             scene == (s32)&SceneId_Ov664Room6 ||
             scene == (s32)&SceneId_Ov664Room7 ||
             scene == (s32)&SceneId_Ov664Room9) {
    gData_020073e4 = x + 64;
    gData_020073e8 = z - y;
  }
  Call6((void (*)())Func_02005db4, gData_020073e4, gData_020073e8,
        gData_020073dc, gData_020073e0, 73, 72);
  Call6((void (*)())Func_02005db4, 70, 72, gData_020073dc, gData_020073e0,
        gData_020073e4, gData_020073e8);
}

void Func_02002898(void) {
  struct Actor *actor = Actor_GetById(Func_02005f84());
  s32 x = actor->x >> 20;
  s32 y = actor->y >> 20;
  s32 z = actor->z >> 20;
  s32 cell = (x << 16) + z;
  s32 scene;

  if (cell == gData_02006c0c)
    return;
  gData_02006c0c = cell;
  gData_020073dc = 1;
  gData_020073e0 = 2;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov664Room1 || scene == (s32)&SceneId_Ov664Room2) {
    gData_020073e4 = x;
    gData_020073e8 = z - y + 64;
  } else if (scene == (s32)&SceneId_Ov664Room5 ||
             scene == (s32)&SceneId_Ov664Room6) {
    gData_020073e4 = x + 64;
    gData_020073e8 = z - y;
  }
  Call6((void (*)())Func_02005db4, gData_020073e4, gData_020073e8,
        gData_020073dc, gData_020073e0, 73, 72);
  Call6((void (*)())Func_02005db4, 71, 72, gData_020073dc, gData_020073e0,
        gData_020073e4, gData_020073e8);
}

/// When the controlled object steps onto a new cell, saves the 1 by 3 cells
/// below it at (73, 72) and stamps the cells kept at (72, 72) in their place.
void Func_02002980(void) {
  struct Actor *object = Actor_GetById(Func_02005f84());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gData_02006c0c) {
    gData_02006c0c = key;
    gData_020073dc = 1;
    gData_020073e0 = 3;
    gData_020073e4 = x;
    gData_020073e8 = z - y + 64;
    CopyCells(x, gData_020073e8, 1, 3, 73, 72);
    CopyCells(72, 72, gData_020073dc, gData_020073e0, gData_020073e4,
              gData_020073e8);
  }
}

/// As Func_02002980, one row nearer.
void Func_02002a14(void) {
  struct Actor *object = Actor_GetById(Func_02005f84());
  s32 x = object->x >> 20;
  s32 y = object->y >> 20;
  s32 z = object->z >> 20;
  s32 key = (x << 16) + z;

  if (key != gData_02006c0c) {
    gData_02006c0c = key;
    gData_020073dc = 1;
    gData_020073e0 = 3;
    gData_020073e4 = x;
    gData_020073e8 = z - y + 63;
    CopyCells(x, gData_020073e8, 1, 3, 73, 72);
    CopyCells(72, 72, gData_020073dc, gData_020073e0, gData_020073e4,
              gData_020073e8);
  }
}

#include "system/field_effect.h"
#include "system/vector3.h"

void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_02005dcc(s32 a, s32 b, s32 c);

void Func_02002aa8(s32 x, s32 z) {
  struct EffectOptions options;
  struct Vector3 spawn;
  s32 i;

  options.priority = 1;
  options.startScaleX = 0x20000;
  options.startScaleY = 0x20000;
  options.targetScaleX = 0x8000;
  options.targetScaleY = 0x8000;
  i = 0;
burst:
    spawn.x = x + ((Random16Far() & 63) << 16) - 0x200000;
    spawn.y = (Random16Far() & 15) << 12;
    spawn.z = z + ((Random16Far() & 7) << 16) - 0x100000;
    Func_020002f4(spawn.x, 0, spawn.z, 0, spawn.y, 0,
                  EFFECT_SCALE_TO_TARGET | EFFECT_USE_START_SCALE |
                      EFFECT_USE_PRIORITY | 1,
                  &options);
  i++;
  if (i != 8)
    goto burst;
  Func_02005dcc(0x50000, 0x70000, 0x10000);
  Call3((void (*)())Func_02005dcc, -1, -1, 0xe666);
}

struct AirsRockInsideVector {
  s32 x;
  s32 y;
  s32 z;
};

struct AirsRockInsideSpawn {
  s32 mode;
  s32 unk04;
  s32 scaleX;
  s32 scaleY;
  s32 speed;
  s32 acceleration;
  u16 kind;
  const u8 *script;
  s32 unk20;
  void *update;
};

void Func_02002b58(s32 x, s32 z) {
  struct AirsRockInsideSpawn spawn;
  struct AirsRockInsideVector position;
  s32 i;

  spawn.kind = 0x2a0;
  spawn.mode = 1;
  spawn.scaleX = 0xcccc;
  spawn.scaleY = 0x30000;
  // A goto loop: the ROM rebuilds the height and the position's address on
  // every pass, which a for loop hoists (differ 55).
  i = 0;
next:
  position.x = x + ((Random16Far() & 31) << 16) - 0x100000;
  position.y = 0x40000;
  position.z = z;
  Func_020002f4(position.x, 0, position.z, 0, position.y, 0, 0x1a0000, &spawn);
  FieldEvent_Wait(5);
  i++;
  if (i != 12)
    goto next;
}

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_02002bd0(struct Actor *object) {
  s32 radius = ((object->y - object->terrainHeight) >> 1) + 0x100000;

  object->x =
      WORD_AT(object, 68) +
      Iwram_MulQ16(Math_Cosine((WORD_AT(object, 48) & 255) << 11), radius);
  object->z =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Math_Sine((WORD_AT(object, 48) & 255) << 11), radius) / 3;
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
}

/// Circles the object around its centre (fields 68 and 76) on an ellipse twice
/// as wide as deep, the radius growing with its height; it rises by field 72
/// each frame and switches its part attribute between 0 and 9 every two frames.
void Func_02002c40(struct Actor *object) {
  s32 y = object->y;
  s32 phase;
  s32 radius;

  phase = WORD_AT(object, 48);
  radius = 0xc0000;
  /* FAKEMATCH(barrier): the radius constant is built before y >> 2, as the
   * native does; as radius = (y >> 2) + 0xc0000, 2 of 56 instructions differ
   * in tla-en. */
  __asm__("");
  radius += y >> 2;
  object->x = WORD_AT(object, 68) +
              Iwram_MulQ16(Math_Cosine((phase & 255) << 11), radius) * 2;
  object->z =
      WORD_AT(object, 76) +
      Iwram_MulQ16(Math_Sine((WORD_AT(object, 48) & 255) << 11), radius);
  object->y += WORD_AT(object, 72);
  WORD_AT(object, 48)++;
  Actor_SetPartAttribute(object, ((gFrameCount & 2) >> 1) * 9);
}

void Func_02002cc0(struct Actor *object) {
  s32 bit = ((gFrameCount & 3) >> BYTE_AT(object, 99)) & 1;

  Actor_SetPartAttribute(object, (u8)(bit * BYTE_AT(object, 98)));
}
