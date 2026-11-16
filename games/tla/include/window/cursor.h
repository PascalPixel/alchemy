#ifndef ALCHEMY_CURSOR_H
#define ALCHEMY_CURSOR_H

/// @file
/// The cursor that approaches its target, shared by shops, the name entry
/// screen, the settings menu and the file screen's sprites.

#include "static_assert.h"
#include "types.h"

struct WindowSprite;

/// Cursor that approaches its target, shared by shops, the name entry screen
/// and the settings menu.
struct MenuCursor {
  struct WindowSprite *anchor; ///< the cursor's sprite, moved with its OAM
  u16 x;       ///< screen pixels: where the current move started
  u16 y;       ///< screen pixels: where the current move started
  s16 targetX; ///< screen pixels
  s16 targetY; ///< screen pixels
  s8 frame;    ///< frames of the current move done; reset when it ends
  s8 frames;   ///< frames the move takes; 0 when the cursor is still
};

STATIC_ASSERT(sizeof(struct MenuCursor) == 0x10);
STATIC_ASSERT(offsetof(struct MenuCursor, targetX) == 0x08);
STATIC_ASSERT(offsetof(struct MenuCursor, frame) == 0x0c);

#endif
