#ifndef ALCHEMY_FOUR_OBJECT_MOTION_H
#define ALCHEMY_FOUR_OBJECT_MOTION_H

#include "types.h"
#include "global_cells.h"
#include "layout_guard.h"

struct FourObjectMotionState {
  u8 unk000[0x144];
  s16 verticalOrigins[4];
  u8 unk14c[0xd8];
  void *objects[4];
  s16 positionsX[4];
  s16 positionsY[4];
  s32 phases[4];
};

LAYOUT_OFFSET_GUARD(FourObjectMotionState_vertical_origins_offset,
                    struct FourObjectMotionState, verticalOrigins, 0x144);
LAYOUT_OFFSET_GUARD(FourObjectMotionState_objects_offset,
                    struct FourObjectMotionState, objects, 0x224);
LAYOUT_OFFSET_GUARD(FourObjectMotionState_positions_x_offset,
                    struct FourObjectMotionState, positionsX, 0x234);
LAYOUT_OFFSET_GUARD(FourObjectMotionState_positions_y_offset,
                    struct FourObjectMotionState, positionsY, 0x23c);
LAYOUT_OFFSET_GUARD(FourObjectMotionState_phases_offset,
                    struct FourObjectMotionState, phases, 0x244);

#endif
