#include "field/perspective.h"

// An actor script's call: turns the actor's sprite to the yaw of the World Map's view (the map
// grid's heap slot holds the WORLD_MAP_VIEW there), clears its sprite flags and goes on with the
// script.
Sint32 scTileSpriteReset(ACTOR *actor)
{
    WORLD_MAP_VIEW *view   = (WORLD_MAP_VIEW *)RamHeapSlots->mapGrid;
    FIELD_SPR      *sprite = actor->sprite;

    sprite->rotation       = view->yaw;
    sprite->flags          = 0;
    return 1;
}
