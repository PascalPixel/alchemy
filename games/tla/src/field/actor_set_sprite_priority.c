// Setting an actor's sprite priority by hand.

#include "field/field.h"

// Fixes the priority of the actor's sprite and its shadow, which then no longer follows the map
// metatile under it (clears ACTOR_PRIORITY_AUTOMATIC).
void fdSetActorSpritePriority(ACTOR *actor, Sint32 priority)
{
    SPR *sprite;

    if (actor == NULL || (actor->spriteStorageKind & ACTOR_SPRITE_KIND_MASK) == ACTOR_SPRITE_NONE)
        return;
    sprite                   = actor->sprite;
    sprite->image.priority   = priority;
    sprite->shadow.priority  = priority;
    actor->priorityFlags    &= ~ACTOR_PRIORITY_AUTOMATIC;
}
