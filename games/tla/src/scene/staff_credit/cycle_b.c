#include "types.h"
#include "frame.h"
#include "actor.h"

void Animation_ApplyChildValues(struct Actor *object, s32 value);

void Func_02001a0c(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Animation_ApplyChildValues(object, (frame >> 1) % 6);
}
