#include "types.h"
#include "field_obj.h"

// Twenty-three scenes, most of those that spawn effects (effect_spawn.h): two
// ways to place a still effect object of a given type at a 16.16 position.

/// Sets the actor's sprite priority (0 in front of the backgrounds, 3 behind).
UNUSED static void SetEffectRecordMode(struct FieldActor *work, s32 mode) {
  work->sprite->image.priority = mode;
}

/// A still, translucent effect in palette 14, at sprite priority 0.
UNUSED static void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type) {
  struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

  if (object != 0) {
    object->sprite->image.priority = 0;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Engine_ActorSetSpriteFlags(object, 0);
    ObjectGroup_SetChildValue(object, 14);
    Engine_ObjectSetBlendMode(object, OBJECT_BLEND_TRANSLUCENT);
    return object;
  }
  return 0;
}

/// A still effect in palette 15, at sprite priority 1, drawn under the actors.
UNUSED static void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z,
                                                         s32 type) {
  struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

  if (object != 0) {
    object->sprite->image.priority = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Engine_ActorSetSpriteFlags(object, 0);
    ObjectGroup_SetChildValue(object, 15);
    object->priorityFlags =
        (object->priorityFlags & ~ACTOR_PRIORITY_AUTOMATIC) |
        ACTOR_PRIORITY_UNDERFOOT;
    return object;
  }
  return 0;
}
