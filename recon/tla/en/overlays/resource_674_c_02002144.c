// Near miss, differ 35 of 58: the listing keeps the second object's x and z in
// r4 and r2 and passes the first pair from r0 and r1 where it tested them; this
// draft allocates them otherwise.
#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_02002144(void) {
  struct ObjectRuntime *object;
  s32 *x0 = &gData_0200519c;
  s32 *z0 = &gData_02005194;
  s32 x1;
  s32 z1;

  object = Object_GetById(12);
  *x0 = object->x;
  *z0 = object->z;
  object = Object_GetById(13);
  gData_02005198 = x1 = object->x;
  gData_02005190 = z1 = object->z;
  if (*x0 >> 20 == 7 && *z0 >> 20 == 29 && x1 >> 20 == 7 && z1 >> 20 == 28)
    Func_020020ec(*x0, *z0, 2, 0);
  if (gData_0200519c >> 20 == 12 && gData_02005194 >> 20 == 25 &&
      gData_02005198 >> 20 == 12 && gData_02005190 >> 20 == 26)
    Func_020020ec(gData_02005198, gData_02005190, 2, 0);
}
