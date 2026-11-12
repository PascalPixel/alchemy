#include "../../field/object/set_sprite_priority.c"

#include "../../field/object/prepare_object.c"

#include "types.h"
#include "actor.h"
#include "event_inline.h"

struct Actor *Func_020000d0(s32 a0, s32 a1, s32 a2, s32 a3) {
  struct Actor *t0;

  t0 = FieldActor_Create(a3, a0, a1, a2);
  if (t0 != 0) {
    Scene_SetActorSpritePriority(t0, 1);
    t0->motionFlags = 0;
    t0->collisionFlags = 8;
    Actor_SetSingleChildFlags(t0, 0);
    Actor_SetSpriteColor(t0, 0xf);
    t0->priorityFlags = 0x22 | t0->priorityFlags;
    return t0;
  }
  return 0;
}
