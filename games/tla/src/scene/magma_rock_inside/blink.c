#include "local.h"

void Func_02000da4(struct Actor *object) {
  if (gFrameCount & object->unk5c[7])
    Actor_SetPartAttribute(object, 10);
  else
    Actor_SetPartAttribute(object, 0);
}
