#include "islets.h"

void Func_02001278(struct Actor *object) {
  struct Actor *source = Actor_GetById(9);

  Actor_SetPositionAndResetMotion(object, source->x, source->y + 0x100000,
                                   source->z);
  object->angle = source->angle;
}
