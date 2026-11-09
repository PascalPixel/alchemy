#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "field_effect.h"
#include "vector3.h"
#include "event_inline.h"

s32 Math_Cosine(s32 angle);
u32 Random16Far(void);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02000478(s32 radius, u32 count) {
  struct Actor *object;
  struct EffectOptions options;
  struct Vector3 dir;
  u32 i;
  s32 angle;

  object = Actor_Get(8);
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  options.priority = ((struct FieldSprite *)object->animation)->priority;
  i = 0;
next:
  angle = i * (0x10000 / (s32)count);
  dir.x = Math_Cosine(angle) * radius / 256;
  dir.y = 0;
  dir.z = Math_Sin(angle) * radius / 256;
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
