// Draft of overlay 6b4's Func_02000478, written from its listing: spreads count
// + 1 effects (Func_020000b8, this overlay's Effect_Spawn) in a ring of the
// given radius around object 8, at its sprite's priority, shrunk to 0.8 and
// moved by Effect_AdvanceMotion. Differ 53 of 104. Left: register allocation
// (the native keeps the options' address in r7, the object in r8 and the
// counter in sl) and the direction's address, which the native forms inside the
// loop after the first call and this before the loop.
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "vector3.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02000478(s32 radius, u32 count) {
  struct ObjectRuntime *object;
  struct EffectOptions options;
  struct Vector3 dir;
  u32 i;
  s32 angle;

  object = Object_GetById(8);
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  options.priority = ((struct FieldSprite *)object->animation)->priority;
  i = 0;
next:
  angle = i * (0x10000 / (s32)count);
  dir.x = Math_Cosine(angle) * radius / 256;
  dir.y = 0;
  dir.z = Math_Sine(angle) * radius / 256;
  dir.x += dir.x >> 1;
  dir.x += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
  dir.z += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
  Func_020000b8(object->x, object->y, object->z, dir.x, dir.y, dir.z,
                EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                    EFFECT_USE_PRIORITY | 1,
                &options);
  i++;
  if (i <= count)
    goto next;
}
