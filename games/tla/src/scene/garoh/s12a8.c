#include "types.h"
#include "field_effect.h"
#include "actor.h"
#include "event_inline.h"

struct Vec {
  s32 x;
  s32 y;
  s32 z;
};

s32 Math_Cosine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_020012a8(s32 objectId) {
  struct Actor *object;
  struct Vec vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  s32 z;

  object = Actor_Get(objectId);
  params.update = Effect_AdvanceMotion;
  // A goto loop: the vector's address is formed after the cosine call on every
  // pass, where a for loop hoists it.
  angleStep = 0;
burst:
  angle = angleStep << 12;
  vec.x = Math_Cosine(angle);
  vec.y = 0;
  z = Math_Sin(angle);
  x = vec.x;
  vec.z = z;
  x += x / 3;
  vec.x = x;
  Func_020000b8(object->x, object->y, object->z, x, vec.y, z, 0x01000001,
                &params);
  angleStep += 2;
  if (angleStep <= 16)
    goto burst;
}
