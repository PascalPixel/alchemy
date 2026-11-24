/// @file
/// Setting an effect's sprite priority by hand: the copy of
/// actor_set_sprite_priority.c that comes with a late field-effect spawner.
/// Anemos Inner Sanctum and the Mars Lighthouse entrance link the first copy
/// early, and near their end a field-effect spawner with this copy just before
/// it; only that spawner calls this one.

#include "types.h"
#include "field/field_event.h"
#include "field/field.h"

/// Fixes the priority of the actor's sprite and its shadow, which then no
/// longer follows the map cell under it (clears ACTOR_PRIORITY_AUTOMATIC).
void FieldEffect_SetSpritePriority(struct Actor *actor, s32 priority) {
  struct Sprite *sprite;

  if (actor == NULL ||
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_NONE)
    return;
  sprite = actor->sprite;
  sprite->image.priority = priority;
  sprite->shadow.priority = priority;
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
}
