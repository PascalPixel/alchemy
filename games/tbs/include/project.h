#ifndef ALCHEMY_PROJECT_H
#define ALCHEMY_PROJECT_H

#include "types.h"
#include "static_assert.h"

struct Projection {
  s32 focal;
  s32 near;
  s32 far;
  s32 centerX;
  s32 centerY;
};

STATIC_ASSERT(sizeof(struct Projection) == 0x14);
STATIC_ASSERT(offsetof(struct Projection, centerX) == 0x0c);
STATIC_ASSERT(offsetof(struct Projection, centerY) == 0x10);

extern struct Projection gProjection;

#endif
