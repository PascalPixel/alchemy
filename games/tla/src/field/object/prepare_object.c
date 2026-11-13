#include "battle/battle.h"
#include "debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field_event.h"

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *object = Import_ActorCreate(type, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 0);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetSpriteColor(object, 14);
    Actor_SetBlendMode(object, 1);
    return object;
  }
  return NULL;
}
