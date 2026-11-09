#include "islets.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02001278(struct Actor *object) {
  struct Actor *source = Actor_Get(9);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y + PIXELS(16),
                                  source->z);
  object->angle = source->angle;
}
