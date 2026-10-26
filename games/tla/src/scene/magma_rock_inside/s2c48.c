#include "types.h"
#include "object_runtime.h"
#include "frame.h"
#include "field_effect.h"

u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02002c48(struct ObjectRuntime *object) {
  struct EffectOptions options;
  s32 position[3];
  u32 frame = gFrameCount & 7;

  if (frame != 0)
    return;
  options.palette = 7;
  options.type = 673;
  options.priority = 1;
  position[0] = object->x + (((Random16Far() & 31) - 16) << 16);
  position[1] = object->y + ((Random16Far() & 31) << 16) + 0x80000;
  position[2] = object->z;
  Func_0200015c(position[0], position[1], position[2] + 0x40000, 0, 0x8000,
                frame, 0x130000, &options);
}
