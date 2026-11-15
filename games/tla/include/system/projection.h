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

/// The projection the battle and the field draw with (IWRAM).
extern struct Projection gProjection;

/// Where the battle centres its projection on the screen: across the middle,
/// and 120 pixels down, below the screen's middle.
#define BATTLE_PROJECTION_CENTER_X (DISPLAY_WIDTH / 2)
#define BATTLE_PROJECTION_CENTER_Y 120

#endif
