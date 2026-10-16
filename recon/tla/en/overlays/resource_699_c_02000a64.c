// Draft, differ 4 of 72: the native copies the object pointer out of r10 before
// it scales the first random number (mov r2, sl; lsls r3, r0, #3; ldr r2, [r2,
// #8]; adds r3, r3, r0); this form scales first. The offset in a temporary, the
// object's x read first, and a one-pass block after the call measured 4 to 12;
// adding in the other order is 41. Overlay 69a's Func_0200090c is the same
// function.
#include "types.h"

#include "object_runtime.h"
#include "frame.h"

u32 Random16Far(void);
void Func_02001094(s32 x, s32 y, s32 z, s32 a, s32 speed, s32 b, s32 c,
                   s32 *params);

s32 Func_02000a64(struct ObjectRuntime *object) {
  s32 params[10];

  if ((gFrameCount & 7) == 0) {
    s32 x;
    s32 y;

    params[0] = 3;
    params[2] = 0xb333;
    params[3] = 0xb333;
    params[1] = 14;
    x = object->x + ((((Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16Far() * 20) >> 16)) << 16);
    Func_02001094(x, y, object->z, 0,
                  ((((Random16Far() * 5) >> 16) << 16) + 0x50000) / 10, 0,
                  0xb0000, params);
  }
  return 0;
}
