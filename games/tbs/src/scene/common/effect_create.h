#include "types.h"
#include "actor.h"

// Twenty-three maps, most of those that spawn effects (effect_spawn.h): two
// ways to place a still effect object of a given type at a 16.16 position.

/// Sets the actor's sprite priority (0 in front of the backgrounds, 3
/// behind).
UNUSED static void SetEffectRecordMode(struct Actor *work, s32 mode) {
  work->sprite->image.priority = mode;
}

/// A still, translucent effect in palette 14, at sprite priority 0.
UNUSED static void *Actor_PrepareObject(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *actor = Actor_Create(type, x, y, z);

  if (actor != 0) {
    actor->sprite->image.priority = 0;
    actor->motionFlags = 0;
    actor->collisionFlags = 8;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_BLACK);
    Actor_SetBlendMode(actor, OBJECT_BLEND_TRANSLUCENT);
    return actor;
  }
  return 0;
}

/// A still effect in palette 15, at sprite priority 1, drawn under the
/// actors.
UNUSED static void *Actor_CreateConfiguredObject(s32 x, s32 y, s32 z,
                                                 s32 type) {
  struct Actor *actor = Actor_Create(type, x, y, z);

  if (actor != 0) {
    actor->sprite->image.priority = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = 8;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags = (actor->priorityFlags & ~ACTOR_PRIORITY_AUTOMATIC) |
                           ACTOR_PRIORITY_UNDERFOOT;
    return actor;
  }
  return 0;
}
