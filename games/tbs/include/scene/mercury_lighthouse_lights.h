#ifndef ALCHEMY_MERCURY_LIGHTHOUSE_LIGHTS_H
#define ALCHEMY_MERCURY_LIGHTHOUSE_LIGHTS_H

/// @file
/// The lights the pool of Mercury Lighthouse and its entrance give the leader,
/// which hold him up as he hops across the water
/// (shared/mercury_lighthouse_lights.c and shared/mercury_lighthouse_pillar.c).

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

struct Actor;

/// The cell behaviors Ply gives the pool's cells in
/// MercuryLighthouseEntrance and MercuryLighthouseLower, which the hops and the
/// lights answer.
enum MercuryLighthousePoolBehavior {
  /// The pool's heart, which gives the leader lights.
  CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART = 11,
  /// Out in the water, where ripples spread from the leader.
  CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER = 21,
  /// A plain stepping square of the pool or the floor at its edge, which takes
  /// the leader's lights.
  CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP = 31
};

/// Published in the scene's slot for the leader's lights while the pillar is
/// active.
struct MercuryLighthouseLights {
  /// The leader's lights, 0 to 3: the heart gives three and each hop into the
  /// pool spends one.
  s32 levels;
  s32 poolBehavior; ///< the pool's cell behavior, the floor's region
  /// Frames to the next ripple in CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER,
  /// 10 to 39; a hop zeroes it.
  s32 rippleTimer;
  /// 16.16 fixed-point pixels: the centre of the column's cell, where a
  /// sinking leader is put back.
  FIXED targetX;
  FIXED targetZ;
  struct Actor *pillar; ///< the column of light over the leader, or null
  /// The light holding the leader up where they landed in the pool, or null.
  struct Actor *stepEffect;
};

STATIC_ASSERT(sizeof(struct MercuryLighthouseLights) == 28);

/// Keeps the lights' state for the floor's region; returning to where the
/// party was, the column and the light under the leader are made again.
void MercuryLighthouse_RestoreLights(s32 region,
                                     struct MercuryLighthouseLights *st);

#endif
