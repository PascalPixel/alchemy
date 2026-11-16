/// @file
/// Creating an actor as a black silhouette.

#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/field_event.h"

/// Creates an actor of sprite type at (x, y, z) to stand as a silhouette: at
/// drawing priority 0, still, skipped by target searches, with no outline or
/// shadow, all black (SPRITE_COLOR_BLACK) and blended.
struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *actor = Import_ActorCreate(type, x, y, z);

  if (actor != NULL) {
    Scene_SetActorSpritePriority(actor, 0);
    actor->motionFlags = 0;
    actor->collisionFlags = 8;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, 14);
    Actor_SetBlendMode(actor, 1);
    return actor;
  }
  return NULL;
}
