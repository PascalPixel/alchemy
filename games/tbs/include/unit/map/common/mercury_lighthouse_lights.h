#ifndef ALCHEMY_UNIT_MAP_COMMON_MERCURY_LIGHTHOUSE_LIGHTS_H
#define ALCHEMY_UNIT_MAP_COMMON_MERCURY_LIGHTHOUSE_LIGHTS_H

#include "types.h"
#include "static_assert.h"

struct ObjectEvent;

/// Published in the map's light-controller slot while the pillar is active.
struct MercuryLighthouseLights {
  /// The leader's lights, 0 to 3: the heart gives three and each hop into the
  /// pool spends one.
  s32 levels;
  s32 terrainCode; ///< the pool's metatile behavior, the floor's region
  /// Frames to the next ripple at trigger 21, 10 to 39; a hop zeroes it.
  s32 rippleTimer;
  /// 16.16 fixed-point pixels: the centre of the column's cell, where a
  /// sinking leader is put back.
  s32 targetX;
  s32 targetZ;                ///< as targetX
  struct ObjectEvent *pillar; ///< the column of light over the leader, or null
  /// The light holding the leader up where they landed in the pool, or null.
  struct ObjectEvent *stepEffect;
};

STATIC_ASSERT(sizeof(struct MercuryLighthouseLights) == 0x1c);

void MercuryLighthouse_RestoreLights(s32 region,
                                     struct MercuryLighthouseLights *st);

#endif
