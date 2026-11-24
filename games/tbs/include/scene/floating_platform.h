#ifndef ALCHEMY_FLOATING_PLATFORM_H
#define ALCHEMY_FLOATING_PLATFORM_H

/// @file
/// The floating platforms of Mercury and Venus Lighthouses' aeries
/// (shared/floating_platform.c).

#include "types.h"

/// Returns the placed actor drawn as a floating platform nearest the leader,
/// or 0.
s32 Actor_FindNearestFloatingPlatform(void);

#endif
