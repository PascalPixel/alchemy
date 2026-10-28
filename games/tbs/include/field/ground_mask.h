#ifndef ALCHEMY_GROUND_MASK_H
#define ALCHEMY_GROUND_MASK_H

/// @file
/// The fade overlay, the weather particles and the actor effect work.

#include "types.h"
#include "edition.h"

#include "sprite.h"

struct Actor;

/// The fade overlay draws one mark on each side of its tracked actor.
struct FadeOverlayWork {
  /// The left and right marks: 16-pixel-wide semi-transparent sprites drawn
  /// where the ground beside the actor rises above its floor.
  struct AnimationSpritePart marks[2];
  struct Actor *actor; ///< the actor tracked; null draws nothing
};

STATIC_ASSERT(sizeof(struct FadeOverlayWork) == 28);

/// HEAP_SLOT_FADE_OVERLAY: the fade overlay's work from
/// FieldEffect_StartFadeOverlay until the next scene change.
extern struct FadeOverlayWork *gFadeOverlayWork;

#define WEATHER_PARTICLE_COUNT 32 ///< WeatherWork.particles

/// One weather particle: OAM attributes followed by its world position.
struct GroundParticle {
  /// The depth-slot queue's link, as AnimationSpritePart.link; cleared with the
  /// attributes and never read by the effects.
  u8 link[4];
  /// The OAM entry's attributes 0 to 2: the start routines write them whole
  /// as semi-transparent, size 1, priority 1 and palette 13.
  ///@{
  u16 y : 8;           ///< Screen pixels.
  u16 affine : 2;      ///< Not named by the effects.
  u16 blendMode : 2;   ///< 1, semi-transparent; not named by the effects.
  u16 mosaic : 1;      ///< Not named by the effects.
  u16 fullColor : 1;   ///< Not named by the effects.
  u16 shape : 2;       ///< From the ground particle frame; motes, sparkles: 0.
  u16 x : 9;           ///< Screen pixels.
  u16 affineIndex : 5; ///< Sparkles: 8 on alternate frames, else 0.
  u16 size : 2;        ///< From the ground particle frame; motes, sparkles: 1.
  u16 tile : 10;       ///< The work's tileBase plus the frame's tile.
  u16 priority : 2;    ///< 1.
  u16 palette : 4;     ///< 13; not named by the effects.
  ///@}
  u8 attr3[2];   ///< OAM attribute 3, cleared with the others.
  s32 positionX; ///< World position, in 16.16 pixels.
  s32 positionY; ///< The ground's height there.
  s32 positionZ; ///< World position, in 16.16 pixels.
  s32 delay;     ///< Motes: height risen; sparkles: phase within a burst.
  u16 timer;     ///< Frames left; zero frees the particle.
};

STATIC_ASSERT(offsetof(struct GroundParticle, positionX) == 0x0c);
STATIC_ASSERT(sizeof(struct GroundParticle) == 0x20);

/// HEAP_SLOT_WEATHER while ground particles, motes or sparkles run.
struct WeatherWork {
  s32 entry;    ///< Resource entry holding the effect's tiles.
  s32 tileBase; ///< Their first tile in VRAM.
  struct GroundParticle particles[WEATHER_PARTICLE_COUNT];
  s32 unused;  ///< Unused by the weather effects.
  s32 stopped; ///< Nonzero stops new sparkle bursts.
};

STATIC_ASSERT(sizeof(struct WeatherWork) == 0x410);

/// HEAP_SLOT_WEATHER: the running ground particles, motes or sparkles.
extern struct WeatherWork *gWeatherWork;

#endif
