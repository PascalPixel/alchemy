#ifndef ALCHEMY_DJINN_FIGURES_H
#define ALCHEMY_DJINN_FIGURES_H

/// @file
/// DjinnFigures, the row of four figures the Status and Djinn menus show.

#include "types.h"
#include "heap_slots.h"
#include "static_assert.h"

struct Sprite;

/// The menu work (HEAP_SLOT_MENU) as the row of four figures reads it: each
/// figure's sprite, screen position and scale.
struct ItemMenuFiguresView {
  /// ItemMenuWork's fields before memberY; not named here.
  u8 beforeMemberY[0x144];
  /// Screen pixels: the party figures' memberY, which moves only the depth
  /// slot.
  s16 verticalOrigins[4];
  /// ItemMenuWork's fields from memberY[4] to completionFlag; not
  /// named here.
  u8 beforeFigures[0xd8];
  struct Sprite *animations[4];
  s16 positionsX[4]; ///< screen pixels
  /// Screen pixels; negative marks a recovering Djinni, which faces
  /// FACING_WEST.
  s16 positionsY[4];
  /// The Djinn menu's figure scales, 16.16: negative holds a scale of -phase,
  /// else it closes a third of its gap to full size (Q16_ONE) each frame.
  s32 phases[4];
};

STATIC_ASSERT(offsetof(struct ItemMenuFiguresView, verticalOrigins) == 0x144);
STATIC_ASSERT(offsetof(struct ItemMenuFiguresView, animations) == 0x224);
STATIC_ASSERT(offsetof(struct ItemMenuFiguresView, positionsX) == 0x234);
STATIC_ASSERT(offsetof(struct ItemMenuFiguresView, positionsY) == 0x23c);
STATIC_ASSERT(offsetof(struct ItemMenuFiguresView, phases) == 0x244);

#endif
