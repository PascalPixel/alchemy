#include "mars_lighthouse_aerie.h"
#include "scene_tables.h"

void Func_020035ec(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(object, object->x - PIXELS(16), object->y,
                                  object->z + PIXELS(32));
}

void Func_0200361c(s32 id) {
  struct Actor *object = Actor_Get(id);

  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetPositionAndResetMotion(object, object->x + PIXELS(16), object->y,
                                  object->z + PIXELS(32));
}
