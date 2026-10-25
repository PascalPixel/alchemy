// Draft, differ 55 of 52: a ring of nine effects spawned at one spot, thrown
// out along every eighth of a turn and a third faster across than along.
#include "types.h"
#include "system/field_effect.h"

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
void Effect_AdvanceMotionLocal(union FieldObject *object);

void Func_02001c9c(void) {
  s32 velocity[3];
  struct EffectOptions options;
  u32 i;

  options.update = Effect_AdvanceMotionLocal;
  i = 0;
loop:
  {
    s32 angle = i << 12;

    velocity[0] = Math_Cosine(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sine(angle);
    velocity[0] += velocity[0] / 3;
    Func_0200015c(0x1d00000, -0x200000, 0x1480000, velocity[0], velocity[1],
                  velocity[2], EFFECT_USE_UPDATE | 1, &options);
    i += 2;
  }
  if (i <= 16)
    goto loop;
}
