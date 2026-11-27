#ifndef ALCHEMY_FLOATING_PLATFORM_H
#define ALCHEMY_FLOATING_PLATFORM_H

/* The floating platforms of Mercury and Venus Lighthouses' aeries
   (shared/floating_platform.c). */

#include "types.h"

/* Returns the placed actor drawn as a floating platform nearest the leader, or
   0. */
Sint32 scActorFindNearestFloatingPlatform(void);

#endif
