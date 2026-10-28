#include "local.h"

void Func_02000690(void) {
  Object_SetModeById(8, 4);
  Object_SetPartAttribute(Object_GetById(8), 6);
  Func_02003d14_(123, 15, 5, 3, 110, 15);
  ((struct SceneEventWork *)Ram_HeapSlots->eventWork)->value16a = 88;
}

void Func_020006d0(void) {
  s32 count = gData_02004d84 - 1;
  s32 row;

  gData_02004d84 = count;
  row = count / 8;
  Func_02003d14((count & 1) * 5 + 118, 47 - row, 5, 2, 110, 47 - row);
  if (gData_02004d84 == 0)
    Task_RemoveFar(Func_020006d0);
}
