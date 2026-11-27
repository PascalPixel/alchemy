#ifndef ALCHEMY_GROUND_MASK_H
#define ALCHEMY_GROUND_MASK_H

// The ground mask's state, and the views of an actor and of the map state that it reads.

#include "types.h"
#include "field/sprite.h"

// The VRAM cache entry holding the ground mask's solid sprite tiles.
#define VRAM_CACHE_ENTRY_FADE_OVERLAY 94

// An actor (ACTOR) as the ground mask reads it: its place, its ground height's halves and its
// layer.
typedef struct FADE_ACTOR
{
    Uint8  unused[8]; // no code names it
    Sint32 x;
    Sint32 y;
    Sint32 z;
    Uint16 groundHeightFraction; // ACTOR.groundHeight's fraction
    Sint16 floor;                // its whole pixels
    Uint8  unusedAfterFloor[10]; // no code names it
    Uint8  terrainId;
} FADE_ACTOR;

// The fade overlay's state (heap slot fadeOverlay): its two marks and the actor they follow.
typedef struct FADE_OVERLAY_STATE
{
    ANIMATION_SPR_PART marks[2];
    FADE_ACTOR        *actor;
} FADE_OVERLAY_STATE;

#endif
