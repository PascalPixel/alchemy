#include "types.h"
#include "call.h"
#include "field_effect.h"
#include "actor.h"

struct Vec {
  s32 x;
  s32 y;
  s32 z;
};

struct Actor *Actor_GetById(s32 id);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02001620(s32 objectId) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 0 of 58 instructions
   * differ in tla-en. */
  struct Actor *object;
  struct Vec vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  s32 z;

  object = Actor_GetById(objectId);
  params.update = Effect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    vec.x = Math_Cosine(angle);
    vec.y = 0;
    z = Math_Sine(angle);
    x = vec.x;
    vec.z = z;
    x += x / 3;
    vec.x = x;
    Func_020000b8(object->x, object->y, object->z, x, vec.y, z,
                  SPLIT_CONSTANT(0x01000001), &params);
  }
}
