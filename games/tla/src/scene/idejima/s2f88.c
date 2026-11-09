#include "types.h"
#include "actor.h"
#include "scene_tables.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);

struct ActorSprite {
  u8 unk00[18];
  u16 rotation;
};

void Func_02002f88(s32 id) {
  struct Actor *object = Actor_GetById(id);

  Actor_SetSingleChildFlags(Actor_GetById(id), 0);
  object->angle = FACING_SOUTH;
  object->motionFlags = 0;
  object->x += 0xc0000;
  object->y = PIXELS(16);
  object->collisionFlags = 0;
  ((struct ActorSprite *)object->animation)->rotation = 0xc000;
}
