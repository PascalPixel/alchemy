// Near miss (16 of 120 instructions differ): three loads of the object's
// position sit one slot earlier or later than the native's, and the native
// stores the turned x after it sums z.
#include "../../../../games/tla/src/scene/sea_of_time/sea_of_time.h"

void Func_0200043c(void) {
  struct SeaOfTimeObject *object = Object_GetById(8);
  s32 turn[1];
  s32 center[3];
  s32 *pos;
  s32 sine;
  s32 dx;
  s32 dz;
  s32 x;
  s32 z;

  center[0] = ((object->x >> 21) << 21) + 0x100000;
  center[2] = ((object->z >> 21) << 21) + 0x100000;
  pos = &object->x;
  if (Func_020003d0(pos, center) > 4) {
    if (*pos < center[0])
      *pos += 0x3333;
    else
      *pos -= 0x3333;
    if (object->z < center[2])
      object->z += 0x3333;
    else
      object->z -= 0x3333;
    if (Func_0200034c(1, center[0] >> 20, center[2] >> 20) >> 8 == 40) {
      turn[0] = SeaOfTime_Cos(-0x200);
      sine = SeaOfTime_Sin(-0x200);
    } else {
      turn[0] = SeaOfTime_Cos(0x200);
      sine = SeaOfTime_Sin(0x200);
    }
    dx = object->x - center[0];
    dz = object->z - center[2];
    x = Iwram_MulQ16(dx, turn[0]) - Iwram_MulQ16(dz, sine);
    z = Iwram_MulQ16(dx, sine) + Iwram_MulQ16(dz, turn[0]);
    object->x = x + center[0];
    object->z = z + center[2];
  }
}
