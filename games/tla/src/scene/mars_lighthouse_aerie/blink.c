#include "mars_lighthouse_aerie.h"

void Func_02001eb0(struct Actor *object) {
  if ((gData_02006b64 >> gData_02006b6c) & 1)
    Actor_SetPartAttribute(object, 15);
  else
    Actor_SetPartAttribute(object, 0);
  gData_02006b64++;
}

void Func_02001ee4(struct Actor *object) {
  if ((gData_02006b64 >> gData_02006b6c) & 1)
    Actor_SetPartAttribute(object, 0);
  else
    Actor_SetPartAttribute(object, 15);
}
