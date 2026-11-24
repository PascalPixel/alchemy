/// @file
/// Creating an actor as a black silhouette.

#include "types.h"
#include "field/field.h"
#include "field/field_event.h"

/// Creates an actor of sprite type at a 16.16 position to stand as a
/// silhouette: at drawing priority 0, still, skipped by target searches, with
/// no outline or shadow, all black and translucent.
struct Actor *Actor_CreateSilhouette(FIXED x, FIXED y, FIXED z, s32 type) {
  struct Actor *actor = Import_ActorCreate(type, x, y, z);

  if (actor != NULL) {
    Scene_SetActorSpritePriority(actor, 0);
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_BLACK);
    Actor_SetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
    return actor;
  }
  return NULL;
}
