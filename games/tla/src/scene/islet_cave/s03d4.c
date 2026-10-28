#include "types.h"
#include "actor.h"
#include "scene_tables.h"

s32 Math_Cosine(s32 angle);

s32 Func_020003d4(struct Actor *object) {
  u8 *phase = (u8 *)object + 0x62;
  s32 step = *phase & 63;
  s16 *animation = object->animation;

  object->motionFlags = 3;
  if (step < 32) {
    if (step == 0)
      object->velocityY = PIXELS(1);
    animation[9] = Math_Cosine(step << 12) / 128;
  }
  (*phase)++;
  return -1;
}
