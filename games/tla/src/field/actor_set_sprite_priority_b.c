/// @file
/// Setting an actor's sprite priority by hand (a second copy).

#include "types.h"
#include "field/field_event.h"

/// Gives the actor's sprite and its shadow drawing priority priority, and
/// stops the camera choosing it from the map.
void Actor_SetSpritePriorityB(struct Actor *object, s32 priority) {
  struct Sprite *sprite;

  if (object == 0 || (object->spriteStorageKind & 15) == 0) {
    return;
  }
  sprite = object->sprite;
  sprite->image.priority = priority;
  sprite->shadow.priority = priority;
  object->priorityFlags &= ~1;
}
