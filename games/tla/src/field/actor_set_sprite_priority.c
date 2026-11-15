/// @file
/// Setting an actor's sprite priority by hand.

#include "types.h"
#include "field/field_event.h"
#include "field/sprite.h"

/// Gives the actor's sprite and its shadow drawing priority priority, and
/// stops the camera choosing it from the map.
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority) {
  struct Sprite *sprite;

  if (object == 0 || (object->spriteStorageKind & 15) == 0) {
    return;
  }
  sprite = object->sprite;
  sprite->image.priority = priority;
  sprite->shadow.priority = priority;
  object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
}
