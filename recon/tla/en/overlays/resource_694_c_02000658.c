// Draft, differ 46 of 56: the logic is right (the three candidate angles are
// masked before the facing is). The native keeps the two position pointers in
// r7 (other) and r5 (object) and the object itself in r8; this form keeps the
// object in r7 and its position pointer in r8, and from there the loads and the
// constants fall in other registers.
#include "types.h"
#include "object_runtime.h"

s32 ArcTan2(s32 y, s32 x);
s32 Func_02000620(s32 *a, s32 *b);

// Test range and whether object faces other within 1/16 turn either side.
s32 Func_02000658(struct ObjectRuntime *object, struct ObjectRuntime *other,
                  s32 range) {
  s32 *to = &other->x;
  s32 *from = &object->x;
  s32 result = 0;

  if (Func_02000620(to, from) < range) {
    u32 angle = (u16)ArcTan2(other->z - object->z, *to - *from);
    u32 ahead = angle + 0x1000;
    u32 behind = angle - 0x1000;
    u32 facing;

    behind &= 0xf000;
    ahead &= 0xf000;
    angle &= 0xf000;
    facing = object->angle & 0xf000;
    if (angle == facing || ahead == facing || behind == facing)
      result = 1;
  }
  return result;
}
