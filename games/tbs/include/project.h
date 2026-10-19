#ifndef ALCHEMY_PROJECT_H
#define ALCHEMY_PROJECT_H

/// @file
/// The 3D projection.

#include "types.h"
#include "static_assert.h"

/// The 3D projection onto the screen: its focal length, near and far depths,
/// and the screen point the view centres on.
struct Projection {
  s32 focal;
  s32 near;
  s32 far;
  s32 centerX; ///< screen pixels
  s32 centerY; ///< screen pixels
};

STATIC_ASSERT(sizeof(struct Projection) == 0x14);
STATIC_ASSERT(offsetof(struct Projection, centerX) == 0x0c);
STATIC_ASSERT(offsetof(struct Projection, centerY) == 0x10);

extern struct Projection gProjection;

#endif
