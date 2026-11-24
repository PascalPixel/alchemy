/// @file
/// Twenty-three scenes, most of those that spawn effects
/// (field_effect_spawn.c): two ways to place a still actor of a given sprite,
/// each taking whole pixels, with an FX form for a 16.16 position.

/// Sets the actor's sprite priority (0 in front of the backgrounds, 3
/// behind).
UNUSED static void Scene_SetActorSpritePriority(struct Actor *actor,
                                                s32 priority) {
  actor->sprite->image.priority = priority;
}

/// A still, translucent black actor in front of the backgrounds (sprite
/// priority 0), at a 16.16 position.
UNUSED static void *Actor_PrepareFX(FIXED x, FIXED y, FIXED z, s32 type) {
  struct Actor *actor = FieldActor_Create(type, x, y, z);

  if (actor != NULL) {
    actor->sprite->image.priority = 0;
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_BLACK);
    Actor_SetBlendMode(actor, ACTOR_BLEND_TRANSLUCENT);
    return actor;
  }
  return NULL;
}

/// Actor_PrepareFX at a point in whole pixels:
/// `Actor_Prepare(464, 0, 290, PIT_MASK)`.
#define Actor_Prepare(x, y, z, type)                                           \
  Actor_PrepareFX(toFIXED(x), toFIXED(y), toFIXED(z), type)

/// A still, invisible actor at sprite priority 1, sorted under the other
/// actors, at a 16.16 position.
UNUSED static void *Actor_CreateConfiguredFX(FIXED x, FIXED y, FIXED z,
                                             s32 type) {
  struct Actor *actor = FieldActor_Create(type, x, y, z);

  if (actor != NULL) {
    actor->sprite->image.priority = 1;
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags = (actor->priorityFlags & ~ACTOR_PRIORITY_AUTOMATIC) |
                           ACTOR_PRIORITY_UNDERFOOT;
    return actor;
  }
  return NULL;
}

/// Actor_CreateConfiguredFX at a point in whole pixels:
/// `Actor_CreateConfigured(128, 0, 420, SPRITE_CRATE)`.
#define Actor_CreateConfigured(x, y, z, type)                                  \
  Actor_CreateConfiguredFX(toFIXED(x), toFIXED(y), toFIXED(z), type)
