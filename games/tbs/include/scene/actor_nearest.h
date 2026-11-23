#ifndef ALCHEMY_ACTOR_NEAREST_H
#define ALCHEMY_ACTOR_NEAREST_H

/// @file
/// The floating platforms of Mercury and Venus Lighthouses' aeries
/// (shared/actor_nearest.c).

#include "types.h"

/// Returns the placed actor drawn as a floating platform nearest the leader,
/// or 0.
s32 Actor_FindNearestFloatingPlatform(void);

#endif
