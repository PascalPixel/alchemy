#ifndef ALCHEMY_PROJECTION_H
#define ALCHEMY_PROJECTION_H

/// @file
/// What games/tla/src/system/system_*.c declare for the files that use it,
/// written once.

#include "types.h"

/// The 3D projection onto the screen: its focal length, near and far depths,
/// and the screen point the view centres on.
struct Projection {
  s32 focal;
  s32 near;
  s32 far;
  s32 centerX;
  s32 centerY;
};

#endif
