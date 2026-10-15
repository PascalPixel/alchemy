#ifndef ALCHEMY_PROJECT_H
#define ALCHEMY_PROJECT_H

#include "types.h"
#include "layout_guard.h"

struct Projection {
  s32 focal;
  s32 near;
  s32 far;
  s32 centerX;
  s32 centerY;
};

LAYOUT_SIZE_GUARD(Projection_Size, struct Projection, 0x14);
LAYOUT_OFFSET_GUARD(Projection_CenterX, struct Projection, centerX, 0x0c);
LAYOUT_OFFSET_GUARD(Projection_CenterY, struct Projection, centerY, 0x10);

extern struct Projection gProjection;

#endif
