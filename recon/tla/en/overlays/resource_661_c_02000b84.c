// Draft, differ 70 of 77: a ring of 17 effects at a point, each thrown
// outward with a random push. Left: register choice (the native keeps the
// options pointer in r7 and the count in r8).
#include "types.h"
#include "field_effect.h"

void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Effect_AdvanceMotionAtLeaderPriority(union FieldObject *object);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
u32 Random16Far(void);

void Func_02000b84(s32 x, s32 y, s32 z) {
  struct EffectOptions options;
  u32 i;

  options.update = Effect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Math_Cosine(i << 14);
    velocity[1] = 0;
    velocity[2] = Math_Sine(i << 14);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((u32)(Random16Far() << 17) >> 16) + -0x10000;
    velocity[2] += (u16)Random16Far() + -0x8000;
    Func_020000b8(x, y, z, velocity[0], velocity[1], velocity[2], 0x01000001,
                  &options);
  }
}
