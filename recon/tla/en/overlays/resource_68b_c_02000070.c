// Under -mthumb-slow-loads (2026-10-26): differ 4 of 56; left: the first
// parameter's copy and its + 8 come after the second's here, before it in the
// native.
// Draft, differ 6 of 56: the distance test and the facing within 0x1000 of
// the angle to the other object. Self's x is taken before other's and the
// left test (angle - 0x1000) declared before the right, with the tests in the
// order ahead, right, left (48 to 6). Left: the native forms other's x pointer
// (r7) before self's (r5), and adds the left offset before it reloads self
// (mov r5, r8). Tried: the pointers assigned in the other order (16-40), an
// empty asm or one-pass block between them (20-42), the angles assigned with
// a barrier between (14-16), permute 6 minutes.
#include "types.h"
#include "actor.h"

s32 Func_02000038(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);

s32 Func_02000070(struct Actor *self, struct Actor *other,
                  s32 range) {
  s32 *from = &self->x;
  s32 *to = &other->x;
  s32 result = 0;

  if (Func_02000038(to, from) < range) {
    u16 angle = ArcTan2(other->z - self->z, *to - *from);
    s32 left = (angle - 0x1000) & 0xf000;
    s32 right = (angle + 0x1000) & 0xf000;
    s32 ahead = angle & 0xf000;
    s32 facing = self->angle & 0xf000;

    if (ahead == facing || right == facing || left == facing)
      result = 1;
  }
  return result;
}
