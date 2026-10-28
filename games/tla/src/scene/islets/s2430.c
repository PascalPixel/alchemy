#include "../../../../games/tla/src/scene/islets/islets.h"

void Func_02002430(void) {
  struct Actor *first = Actor_GetById(12);
  struct Actor *second = Actor_GetById(13);

  gData_0200519c = first->x;
  gData_02005194 = first->z;
  gData_02005198 = second->x;
  gData_02005190 = second->z;
  Func_02003e64();
  Func_02003fa4(0);
  if (gData_0200519c >> 20 == 7 && gData_02005194 >> 20 == 29 &&
      gData_02005198 >> 20 == 7 && gData_02005190 >> 20 == 28) {
    ((u8 *)first)[0x62] = 0;
    ((void (*)())Func_020020ec)(gData_0200519c, gData_02005194, 2);
  }
  Func_02003fe4();
  Func_020021d0();
  Func_02003e6c();
}
