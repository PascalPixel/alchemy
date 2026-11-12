#include "types.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02002b04(u32 id, s8 mode) {
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 0;
  object->priorityFlags |= 0x80;
  object->acceleration = 0x1000;
  object->speedLimit = 0x10000;
  object->y = PIXELS(16);
  object->timer = object->x >> 16;
  object->unk66 = object->z >> 16;
  Actor_SetAnimation(id, mode);
}
