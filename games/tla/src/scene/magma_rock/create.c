#include "../../field/object/set_sprite_priority.c"

#include "../../field/object/prepare_object.c"

#include "types.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"
#include "module/scene/scene.h"

struct Actor *Func_020000d0(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = (FieldActor_Create(kind, x, y, z));

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSingleChildFlags(object, 0);
    FieldActor_SetSpriteColor(object, 15);
    object->priorityFlags |= 34;
    return object;
  }
  return NULL;
}
