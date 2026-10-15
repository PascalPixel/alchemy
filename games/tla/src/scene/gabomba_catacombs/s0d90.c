#include "local.h"

void Func_02000d90(void) {
  struct ObjectRuntime *object = Object_GetById(64);

  if (object != NULL && object->x != 0)
    Func_02001234(0x1040);
}
