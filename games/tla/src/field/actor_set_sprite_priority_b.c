/// @file
/// Setting an actor's sprite priority by hand: a second copy of
/// actor_set_sprite_priority.c under another name, which Anemos Inner Sanctum
/// and the Mars Lighthouse entrance keep beside the first.

#include "types.h"
#include "field/field_event.h"
#include "field/field.h"

/// Fixes the priority of the actor's sprite and its shadow, which then no
/// longer follows the map cell under it (clears ACTOR_PRIORITY_AUTOMATIC).
void Actor_SetSpritePriorityB(struct Actor *actor, s32 priority) {
  struct Sprite *sprite;

  if (actor == NULL ||
      (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_NONE)
    return;
  sprite = actor->sprite;
  sprite->image.priority = priority;
  sprite->shadow.priority = priority;
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
}
