// Draft of overlay 6a5's Func_020045a4, the twin of overlay 69a's
// Func_0200090c: its draft (resource_69a_c_0200090c.c) with this overlay's
// symbols: differ 4 of 72. Left: what that draft's header records.
#include "types.h"
#include "frame.h"
#include "field_event.h"

u32 Random16Far(void);
void Func_02004bd4(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d,
                   s32 *params);

s32 Func_020045a4(struct FieldActor *object) {
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
    Func_02004bd4(x, y, object->z, 0,
                  (s32)((((Random16Far() * 5) >> 16) << 16) + 0x50000) / 10, 0,
                  0xb0000, params);
  }
  return 0;
}
