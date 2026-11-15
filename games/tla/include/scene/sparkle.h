#ifndef ALCHEMY_SPARKLE_H
#define ALCHEMY_SPARKLE_H

/// @file
/// The sparkle that rises round an object while it uses Psynergy in an event
/// (Contigo, Mars Lighthouse's aerie, the Psynergy test): each scene keeps its
/// own copy of the routines.

#include "types.h"
#include "field/effect_sprite.h"

struct Actor;

/// The sparkle over an object: its two pairs of sprites, how far each pair has
/// risen (from 0 to 31; 999 before it starts) and the pair to start next.
struct SparkleWork {
  struct EffectSprite sprites[4];
  s16 heights[2];
  s16 vramEntry;
  s16 nextPair;
  struct Actor *object;
};

/// The sparkle's frame counter in Actor.riseEnabled: bit 0 alternates, the
/// rest picks the colour.
struct CounterBits {
  u8 low : 1;
  u8 rest : 7;
};

#endif
