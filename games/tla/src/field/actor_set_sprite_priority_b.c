/// @file
/// Setting an actor's sprite priority by hand (a second copy).

#include "types.h"
#include "field/field_event.h"
#include "field/field.h"

/// Fixes the priority of the actor's sprite and its shadow, which then no
/// longer follows the map cell under it (clears ACTOR_PRIORITY_AUTOMATIC).
// The same routine as Scene_SetActorSpritePriority, under another name.
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
