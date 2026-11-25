#ifndef ALCHEMY_GROUND_MASK_H
#define ALCHEMY_GROUND_MASK_H

/// @file
/// The ground mask's state, and the views of an actor and of the map state
/// that it reads.

#include "types.h"
#include "field/sprite.h"

/// The VRAM cache entry holding the ground mask's solid sprite tiles.
#define VRAM_CACHE_ENTRY_FADE_OVERLAY 94

/// An actor (struct Actor) as the ground mask reads it: its place, its ground
/// height's halves and its layer.
struct FadeActor {
  u8 unused[8]; ///< no code names it
  s32 x;
  s32 y;
  s32 z;
  u16 groundHeightFraction; ///< Actor.groundHeight's fraction
  s16 floor;                ///< its whole pixels
  u8 unusedAfterFloor[10];  ///< no code names it
  u8 terrainId;
};

/// The fade overlay's state (heap slot fadeOverlay): its two marks and the
/// actor they follow.
struct FadeOverlayState {
  struct AnimationSpritePart marks[2];
  struct FadeActor *actor;
};

#endif
