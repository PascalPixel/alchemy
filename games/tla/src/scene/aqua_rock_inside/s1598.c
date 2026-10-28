#include "types.h"
#include "actor.h"
#include "system/field_effect.h"

extern u32 gFrameCount;

u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02001598(struct Actor *object) {
  s32 phase = gFrameCount & 7;

  if (phase == 0) {
    struct EffectOptions options;
    s32 position[3];

    options.palette = 7;
    options.type = 0x2a1;
    options.priority = 1;
    position[0] = object->x + (((Random16Far() & 31) - 16) << 16);
    position[1] = object->y + ((Random16Far() & 31) << 16) + 0x80000;
    position[2] = object->z;
    Func_0200015c(
        position[0], position[1], position[2] + 0x40000, 0, 0x8000, phase,
        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
}
