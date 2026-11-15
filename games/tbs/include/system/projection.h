#ifndef ALCHEMY_PROJECTION_H
#define ALCHEMY_PROJECTION_H

/// @file
/// The 3D projection.

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "system/io_reg.h"

/// The 3D projection onto the screen: its focal length, near and far depths,
/// and the screen point the view centres on.
struct Projection {
  /// The focal length in 16.16 pixels: 80, half the screen height, over the
  /// tangent of half the vertical field of view (Projection_SetFieldOfView).
  FIXED focal;
  FIXED near;  ///< nearer points are not drawn; 16.16 along the view axis
  FIXED far;   ///< farther points are not drawn; 16.16 along the view axis
  s32 centerX; ///< screen pixels
  s32 centerY; ///< screen pixels
};

STATIC_ASSERT(sizeof(struct Projection) == 0x14);
STATIC_ASSERT(offsetof(struct Projection, centerX) == 0x0c);
STATIC_ASSERT(offsetof(struct Projection, centerY) == 0x10);

/// The current 3D projection: Projection_SetFieldOfView and Projection_Set
/// write its focal length and depth range, and the battle and its battle
/// effects set its centre directly.
extern struct Projection gProjection;

/// Where the battle centres its projection on the screen: across the middle,
/// and 120 pixels down, below the screen's middle.
#define BATTLE_PROJECTION_CENTER_X (DISPLAY_WIDTH / 2)
#define BATTLE_PROJECTION_CENTER_Y 120

#endif
