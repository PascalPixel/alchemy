#include "types.h"
#include "actor.h"

s32 Math_Cosine(s32 angle);

s32 Func_02000058(struct Actor *object) {
  u8 *phase = (u8 *)object + 0x62;
  s32 step = *phase & 63;
  s16 *animation = object->animation;

  object->motionFlags = 3;
  if (step == 0)
    object->velocityY = 0x30000;
  if (step == 16)
    object->velocityY = 0x20000;
  if (step == 24)
    object->velocityY = 0x30000;
  animation[9] = Math_Cosine(step << 12) / 64;
  (*phase)++;
  return -1;
}
