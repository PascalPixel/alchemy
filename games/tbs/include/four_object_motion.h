#ifndef ALCHEMY_FOUR_OBJECT_MOTION_H
#define ALCHEMY_FOUR_OBJECT_MOTION_H

#include "types.h"
#include "global_cells.h"
#include "static_assert.h"

struct AnimationObject;

struct FourObjectMotionState {
  u8 unk000[0x144];
  s16 verticalOrigins[4];
  u8 unk14c[0xd8];
  struct AnimationObject *objects[4];
  s16 positionsX[4];
  s16 positionsY[4];
  s32 phases[4];
};

STATIC_ASSERT(offsetof(struct FourObjectMotionState, verticalOrigins) == 0x144);
STATIC_ASSERT(offsetof(struct FourObjectMotionState, objects) == 0x224);
STATIC_ASSERT(offsetof(struct FourObjectMotionState, positionsX) == 0x234);
STATIC_ASSERT(offsetof(struct FourObjectMotionState, positionsY) == 0x23c);
STATIC_ASSERT(offsetof(struct FourObjectMotionState, phases) == 0x244);

#endif
