#include "types.h"
#include "actor.h"

// Twenty-three scenes, most of those that spawn effects (field_effect_spawn.h):
// two ways to place a still field effect of a given type, each taking whole
// pixels, with a Fixed form for a 16.16 position.

/// Sets the actor's sprite priority (0 in front of the backgrounds, 3
/// behind).
UNUSED static void SetEffectRecordMode(struct Actor *work, s32 mode) {
  work->sprite->image.priority = mode;
}

/// A still, translucent effect in palette 14, at sprite priority 0, at a 16.16
/// position.
UNUSED static void *Actor_PrepareFixed(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *actor = FieldActor_Create(type, x, y, z);

  if (actor != 0) {
    actor->sprite->image.priority = 0;
    actor->motionFlags = 0;
    actor->collisionFlags = 8;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_BLACK);
    Actor_SetBlendMode(actor, ACTOR_BLEND_TRANSLUCENT);
    return actor;
  }
  return 0;
}

/// Actor_PrepareFixed at a point in whole pixels:
/// `Actor_Prepare(464, 0, 290, PIT_MASK)`.
#define Actor_Prepare(x, y, z, type)                                           \
  Actor_PrepareFixed(px(x), px(y), px(z), type)

/// A still effect in palette 15, at sprite priority 1, drawn under the
/// actors, at a 16.16 position.
UNUSED static void *Actor_CreateConfiguredFixed(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *actor = FieldActor_Create(type, x, y, z);

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

/// Actor_CreateConfiguredFixed at a point in whole pixels:
/// `Actor_CreateConfigured(128, 0, 420, SPRITE_CRATE)`.
#define Actor_CreateConfigured(x, y, z, type)                                  \
  Actor_CreateConfiguredFixed(px(x), px(y), px(z), type)
