#ifndef ALCHEMY_MODULE_SCENE_SHARED_MERCURY_LIGHTHOUSE_LIGHTS_H
#define ALCHEMY_MODULE_SCENE_SHARED_MERCURY_LIGHTHOUSE_LIGHTS_H

#include "types.h"
#include "static_assert.h"

struct Actor;

/// The cell behaviors Ply gives the pool's cells in
/// MercuryLighthouseEntrance and MercuryLighthouseLower, which the hops and the
/// lights answer.
enum MercuryLighthousePoolBehavior {
  /// The pool's heart, which gives the leader lights.
  /// ★ Named from what the player sees: the heart at the pool's centre (its
  /// render)
  CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART = 11,
  /// Out in the water, where ripples spread from the leader.
  /// ★ Named from what the player sees: the open water of the pool (its render)
  CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER = 21,
  /// The pool's rim, which takes the leader's lights.
  /// ★ Named from what the player sees: the rim of the pool (its render)
  CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_RIM = 31
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
  s32 targetX;
  s32 targetZ;          ///< as targetX
  struct Actor *pillar; ///< the column of light over the leader, or null
  /// The light holding the leader up where they landed in the pool, or null.
  struct Actor *stepEffect;
};

STATIC_ASSERT(sizeof(struct MercuryLighthouseLights) == 0x1c);

void MercuryLighthouse_RestoreLights(s32 region,
                                     struct MercuryLighthouseLights *st);

#endif
