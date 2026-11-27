// Creating an actor as a black silhouette.

#include "field/field_mode.h"
#include "field/field.h"
#include "field/field_script.h"

// Creates an actor of sprite type at a 16.16 position to stand as a silhouette: at drawing priority
// 0, still, skipped by target searches, with no outline or shadow, all black and translucent.
ACTOR *fdActorCreateSilhouette(FIXED x, FIXED y, FIXED z, Sint32 type)
{
    ACTOR *actor = syImportActorCreate(type, x, y, z);

    if (actor != NULL)
    {
        fdSetActorSpritePriority(actor, 0);
        actor->motionFlags    = 0;
        actor->collisionFlags = ACTOR_NOT_TARGETED;
        fdActorSetSpriteFlags(actor, 0);
        fdActorSetSpriteColor(actor, SPRITE_COLOR_BLACK);
        fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
        return actor;
    }
    return NULL;
}
