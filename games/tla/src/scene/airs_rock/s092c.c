#include "types.h"
#include "actor.h"

extern u32 gFrameCount;

void Actor_SetPartAttribute(struct Actor *object, s32 value);

void Func_0200092c(struct Actor *object) {
  u8 *bytes = (u8 *)object;
  u32 on = ((gFrameCount & 3) >> bytes[99]) & 1;

  Actor_SetPartAttribute(object, (u8)(on * bytes[98]));
}
