// Near miss, differ 4 of 72 (overlays 69B's Func_0200102c and 6A4's
// Func_02000de0 are the same routine). After the first random call the native
// interleaves the x load with the multiply by nine (mov r2, sl; lsls r3, r0,
// #3; ldr r2, [r2, #8]; adds r3, r3, r0) and copies x to r8 just before the
// add; this form scales first (lsls; adds; mov; ldr). Measured: the sum
// written the other way round (differ 41); the random in a local with x read
// first behind an empty asm barrier or a one-pass block (differ 12: x loads
// through r3 and the 0x50000 moves to r3); a "+l" barrier on x (differ 27);
// the product as its shift and add around the load (differ 6), with an empty
// barrier after the load (differ 4: ldr before lsls, mov r8 before adds) or
// before it (differ 6).
#include "types.h"
#include "frame.h"
#include "field_event.h"

u32 Random16Far(void);
void Func_02000f3c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d,
                   s32 *params);

s32 Func_0200090c(struct FieldActor *object) {
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
    Func_02000f3c(x, y, object->z, 0,
                  (s32)((((Random16Far() * 5) >> 16) << 16) + 0x50000) / 10, 0,
                  0xb0000, params);
  }
  return 0;
}
