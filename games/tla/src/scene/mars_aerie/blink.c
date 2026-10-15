#include "mars_aerie.h"

void Func_02001eb0(struct ObjectRuntime *object) {
  if ((gData_02006b64 >> gData_02006b6c) & 1)
    Object_SetPartAttribute(object, 15);
  else
    Object_SetPartAttribute(object, 0);
  gData_02006b64++;
}

void Func_02001ee4(struct ObjectRuntime *object) {
  if ((gData_02006b64 >> gData_02006b6c) & 1)
    Object_SetPartAttribute(object, 0);
  else
    Object_SetPartAttribute(object, 15);
}
