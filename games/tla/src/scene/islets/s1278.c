#include "islets.h"
#include "scene_tables.h"

void Func_02001278(struct Actor *object) {
  struct Actor *source = Actor_GetById(9);

  Actor_SetPositionAndResetMotion(object, source->x, source->y + PIXELS(16),
                                  source->z);
  object->angle = source->angle;
}
