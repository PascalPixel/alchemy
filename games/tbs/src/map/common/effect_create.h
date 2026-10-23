#include "types.h"
#include "object_event.h"

// Twenty-three maps, most of those that spawn effects (effect_spawn.h): two
// ways to place a still effect object of a given type at a 16.16 position.

/// Sets the object event's sprite priority (0 in front of the backgrounds, 3
/// behind).
UNUSED static void SetEffectRecordMode(struct ObjectEvent *work, s32 mode) {
  work->sprite->image.priority = mode;
}

/// A still, translucent effect in palette 14, at sprite priority 0.
UNUSED static void *ObjectEvent_PrepareObject(s32 x, s32 y, s32 z, s32 type) {
  struct ObjectEvent *objectEvent = ObjectEvent_Create(type, x, y, z);

  if (objectEvent != 0) {
    objectEvent->sprite->image.priority = 0;
    objectEvent->motionFlags = 0;
    objectEvent->collisionFlags = 8;
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    ObjectGroup_SetChildValue(objectEvent, 14);
    ObjectEvent_SetBlendMode(objectEvent, OBJECT_BLEND_TRANSLUCENT);
    return objectEvent;
  }
  return 0;
}

/// A still effect in palette 15, at sprite priority 1, drawn under the object
/// events.
UNUSED static void *ObjectEvent_CreateConfiguredObject(s32 x, s32 y, s32 z,
                                                       s32 type) {
  struct ObjectEvent *objectEvent = ObjectEvent_Create(type, x, y, z);

  if (objectEvent != 0) {
    objectEvent->sprite->image.priority = 1;
    objectEvent->motionFlags = 0;
    objectEvent->collisionFlags = 8;
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    ObjectGroup_SetChildValue(objectEvent, 15);
    objectEvent->priorityFlags =
        (objectEvent->priorityFlags & ~OBJ_EVENT_PRIORITY_AUTOMATIC) |
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    return objectEvent;
  }
  return 0;
}
