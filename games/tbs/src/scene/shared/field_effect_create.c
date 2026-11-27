// Twenty-three scenes, most of those that spawn effects (field_effect_spawn.c): two ways to place a
// still actor of a given sprite, each taking whole pixels, with an FX form for a 16.16 position.

// Sets the actor's sprite priority (0 in front of the backgrounds, 3 behind).
UNUSED static void scSetActorSpritePriority(ACTOR *actor, Sint32 priority)
{
    actor->sprite->image.priority = priority;
}

// A still, translucent black actor in front of the backgrounds (sprite priority 0), at a 16.16
// position.
UNUSED static void *scActorPrepareFX(FIXED x, FIXED y, FIXED z, Sint32 type)
{
    ACTOR *actor = syFieldActorCreate(type, x, y, z);

    if (actor != NULL)
    {
        actor->sprite->image.priority = 0;
        actor->motionFlags            = 0;
        actor->collisionFlags         = ACTOR_NOT_TARGETED;
        fdActorSetSpriteFlags(actor, 0);
        fdActorSetSpriteColor(actor, SPRITE_COLOR_BLACK);
        fdActorSetBlendMode(actor, ACTOR_BLEND_TRANSLUCENT);
        return actor;
    }
    return NULL;
}

// scActorPrepareFX at a point in whole pixels: `fdActorPrepare(464, 0, 290, PIT_MASK)`.
#define fdActorPrepare(x, y, z, type) scActorPrepareFX(toFIXED(x), toFIXED(y), toFIXED(z), type)

// A still, invisible actor at sprite priority 1, sorted under the other actors, at a 16.16
// position.
UNUSED static void *scActorCreateConfiguredFX(FIXED x, FIXED y, FIXED z, Sint32 type)
{
    ACTOR *actor = syFieldActorCreate(type, x, y, z);

    if (actor != NULL)
    {
        actor->sprite->image.priority = 1;
        actor->motionFlags            = 0;
        actor->collisionFlags         = ACTOR_NOT_TARGETED;
        fdActorSetSpriteFlags(actor, 0);
        fdActorSetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
        actor->priorityFlags =
            (actor->priorityFlags & ~ACTOR_PRIORITY_AUTOMATIC) | ACTOR_PRIORITY_UNDERFOOT;
        return actor;
    }
    return NULL;
}

// scActorCreateConfiguredFX at a point in whole pixels: `fdActorCreateConfigured(128, 0, 420,
// SPRITE_CRATE)`.
#define fdActorCreateConfigured(x, y, z, type)                                                     \
    scActorCreateConfiguredFX(toFIXED(x), toFIXED(y), toFIXED(z), type)
