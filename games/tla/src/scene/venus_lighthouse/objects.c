#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "event_inline.h"

#define Actor_SetSpriteFlags                                                   \
  ((void (*)(struct Actor *, s32))Actor_SetSpriteFlags)

#define Actor_SetSpriteColor                                                   \
  ((void (*)(struct Actor *, s32))Actor_SetSpriteColor)

void ConfigureOverlayObject(struct Actor *object, s32 parameter) {
  object->motionFlags = 0;
  object->collisionFlags = 8;
  Actor_SetSpriteFlags(object, 0);
  FieldActor_SetSpriteColor(object, parameter);
}

struct Actor *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = FieldActor_Create(kind, x, y, z);

  if (object != 0) {
    Scene_SetActorSpritePriority(object, 0);
    ConfigureOverlayObject(object, 14);
    Actor_SetBlendMode(object, 1);
    return object;
  }
  return 0;
}

struct Actor *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z,
                                                       s32 kind) {
  struct Actor *object = FieldActor_Create(kind, x, y, z);

  if (object != 0) {
    Scene_SetActorSpritePriority(object, 1);
    ConfigureOverlayObject(object, 15);
    object->priorityFlags |= 2;
    return object;
  }
  return 0;
}
