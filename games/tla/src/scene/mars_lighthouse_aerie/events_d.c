#include "mars_lighthouse_aerie.h"

void Func_020035ec(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  ObjectMotion_SetActionVariant(id, 1);
  Object_SetPositionAndResetMotion(object, object->x - 0x100000, object->y,
                                   object->z + 0x200000);
}

void Func_0200361c(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  ObjectMotion_SetActionVariant(id, 1);
  Object_SetPositionAndResetMotion(object, object->x + 0x100000, object->y,
                                   object->z + 0x200000);
}
