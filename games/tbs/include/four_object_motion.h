#ifndef ALCHEMY_FOUR_OBJECT_MOTION_H
#define ALCHEMY_FOUR_OBJECT_MOTION_H

/// @file
/// FourObjectMotion, the row of four figures the Status and Djinn menus show.

#include "types.h"
#include "global_cells.h"
#include "static_assert.h"

struct AnimationObject;

/// The menu work (HEAP_SLOT_MENU) as the row of four figures reads it: each
/// figure's object, screen position and scale.
struct FourObjectMotionState {
  /// ItemMenuWork's fields before ownerY; not named here.
  u8 beforeOwnerY[0x144];
  /// Screen pixels: the party figures' ownerY, which moves only the depth
  /// slot.
  s16 verticalOrigins[4];
  /// ItemMenuWork's fields from ownerY[4] to completionFlag; not
  /// named here.
  u8 beforeFigures[0xd8];
  struct AnimationObject *animations[4];
  s16 positionsX[4]; ///< screen pixels
  /// Screen pixels; negative marks a recovering Djinni, which faces 0x8000.
  s16 positionsY[4];
  /// The Djinn menu's figure scales, 16.16: negative holds a scale of -phase,
  /// else it closes a third of its gap to full size (0x10000) each frame.
  s32 phases[4];
};

STATIC_ASSERT(offsetof(struct FourObjectMotionState, verticalOrigins) == 0x144);
STATIC_ASSERT(offsetof(struct FourObjectMotionState, animations) == 0x224);
STATIC_ASSERT(offsetof(struct FourObjectMotionState, positionsX) == 0x234);
STATIC_ASSERT(offsetof(struct FourObjectMotionState, positionsY) == 0x23c);
STATIC_ASSERT(offsetof(struct FourObjectMotionState, phases) == 0x244);

#endif
