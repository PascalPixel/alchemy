#include "types.h"
#include "actor.h"

s32 Func_02000038(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);

s32 Func_02000070(struct Actor *self, struct Actor *other, s32 range) {
  s32 result = 0;
  s32 *to;
  s32 *from;

  to = &other->x;
  from = &self->x;
  if (Func_02000038(to, from) < range) {
    u32 angle = (u16)ArcTan2(other->z - self->z, *to - *from);
    u32 left = (angle - 0x1000) & 0xf000;
    u32 right = (angle + 0x1000) & 0xf000;
    u32 ahead = angle & 0xf000;
    u32 facing = self->angle & 0xf000;

    if (ahead == facing || right == facing || left == facing)
      result = 1;
  }
  return result;
}
