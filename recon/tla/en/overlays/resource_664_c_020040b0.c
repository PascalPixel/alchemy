// Draft, differ 54 of 63: every eighth frame, spawns a rising effect of type
// 673 (palette 7, priority 1) at a random point up to 16 pixels either side of
// the object and up to 32 pixels beyond it. The same listing as overlay 67c's
// Func_02001598, 681's Func_02001548 and 692's Func_02002c48. Left: instruction
// order and registers around the two random calls.
#include "types.h"
#include "actor.h"
#include "system/field_effect.h"
#include "system/vector3.h"

extern u32 gFrameCount;
u16 Random16Far(void);
void Func_020002f4(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_020040b0(struct Actor *object) {
  struct EffectOptions options;
  struct Vector3 position;
  s32 frame = gFrameCount & 7;

  if (frame == 0) {
    options.palette = 7;
    options.type = 673;
    options.priority = 1;
    position.x = object->x + (((Random16Far() & 31) - 16) << 16);
    position.y = object->y + ((Random16Far() & 31) << 16) + 0x80000;
    position.z = object->z;
    Func_020002f4(
        position.x, position.y, position.z + 0x40000, 0, 0x8000, frame,
        EFFECT_USE_TYPE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE, &options);
  }
}
