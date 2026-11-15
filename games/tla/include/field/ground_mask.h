#ifndef ALCHEMY_GROUND_MASK_H
#define ALCHEMY_GROUND_MASK_H

/// @file
/// What The Lost Age's code declares for the files that use it, written once.

#include "types.h"
#include "field/sprite.h"

struct FadeActor;

/// The VRAM cache entry holding the ground mask's solid sprite tiles.
#define VRAM_CACHE_ENTRY_FADE_OVERLAY 94

/// The fade overlay's work (heap slot fadeOverlay): its two marks and the
/// actor they follow.
struct FadeOverlayWork {
  struct AnimationSpritePart marks[2];
  struct FadeActor *actor;
};

/// A ground particle: an OAM entry linked by depth, its place (16.16 pixels),
/// its delay and its frame count.
struct GroundParticle {
  u8 unused[4]; ///< no code names it
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unusedAfterPalette[2]; ///< no code names it
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 delay;
  u16 timer;
  u8 unusedAfterTimer[2]; ///< no code names it
};

#endif
