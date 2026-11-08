#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

void Func_02000668(struct Actor *object, s32 step) {
  s32 speed = (object->velocityX + object->velocityZ) >> 1;
  s32 *counter = &gData_02006294;

  if (speed < 0)
    speed = -speed;
  if ((u32)speed > 0xbb80)
    *counter += 4;
  else if (speed != 0)
    *counter += 2;
  gData_02006294 += step;
  if (gData_02006294 > 40) {
    Func_020006b0(object);
    gData_02006294 = 0;
  }
}
