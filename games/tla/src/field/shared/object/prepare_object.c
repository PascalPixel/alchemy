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

#define Actor_SetSingleChildFlags                                     \
  ((void (*)(struct FieldActor *, s32))Actor_SetSingleChildFlags)

#define Actor_SetPartAttribute                                                \
  ((void (*)(struct FieldActor *, s32))Actor_SetPartAttribute)

struct FieldActor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type) {
  struct FieldActor *object = Import_ActorCreate(type, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 0);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSingleChildFlags(object, 0);
    Actor_SetPartAttribute(object, 14);
    Animation_SetStateFlags(object, 1);
    return object;
  }
  return NULL;
}
