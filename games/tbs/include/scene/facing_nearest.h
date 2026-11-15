#ifndef ALCHEMY_FACING_NEAREST_H
#define ALCHEMY_FACING_NEAREST_H

/// @file
/// A facing rounded to the directions a scene's pushes, jumps and searches work
/// in.

#include "fixed_math.h"

/// A facing rounded to the nearest of east, south, west and north.
#define FACING_NEAREST_CARDINAL(facing) (((facing) + TRIG_TURN / 8) & 0xc000)
/// A facing rounded to the nearest eighth of a turn.
#define FACING_NEAREST_EIGHTH(facing) (((facing) + TRIG_TURN / 16) & 0xe000)
/// The sixteenth of a turn a facing lies in, what is past it dropped.
#define FACING_SIXTEENTH(facing) ((facing) & 0xf000)

#endif
