#ifndef ALCHEMY_PROJECTION_H
#define ALCHEMY_PROJECTION_H

/// @file
/// The 3D projection the battle and the field draw with
/// (Projection_SetFieldOfView, Matrix_ProjectPoint).

#include "fixed_math.h"
#include "types.h"

/// The 3D projection onto the screen: its focal length, near and far depths,
/// and the screen point the view centres on.
struct Projection {
  FIXED focal; ///< 16.16 pixels
  FIXED near;  ///< nearest depth drawn, 16.16 pixels
  FIXED far;   ///< farthest depth drawn, 16.16 pixels
  s32 centerX; ///< screen pixels
  s32 centerY; ///< screen pixels
};

/// The projection the battle and the field draw with (IWRAM).
extern struct Projection gProjection;

/// Where the battle centres its projection on the screen: across the middle,
/// and 120 pixels down, below the screen's middle.
#define BATTLE_PROJECTION_CENTER_X (DISPLAY_WIDTH / 2)
#define BATTLE_PROJECTION_CENTER_Y 120

#endif
