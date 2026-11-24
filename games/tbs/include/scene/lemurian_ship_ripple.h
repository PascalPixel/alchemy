#ifndef ALCHEMY_LEMURIAN_SHIP_RIPPLE_H
#define ALCHEMY_LEMURIAN_SHIP_RIPPLE_H

/// @file
/// The ripple of the water under the Lemurian ship, a wave in the background's
/// scrolling changed line by line (shared/lemurian_ship_ripple.c).

#include "fixed_math.h"
#include "types.h"

/// Starts the ripple: for x and y, the wave's frequency, its step each frame
/// and its amplitude.
void LineScroll_InitWave(s32 mode, FIXED freqX, s32 stepX, FIXED ampX,
                         FIXED freqY, s32 stepY, FIXED ampY);

#endif
