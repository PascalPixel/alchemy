#ifndef ALCHEMY_CURSOR_H
#define ALCHEMY_CURSOR_H

/// @file
/// The cursor that approaches its target, shared by shops, the
/// name entry screen and the settings menu.

#include "types.h"
#include "static_assert.h"
#include "render_input.h"

/// Cursor that approaches its target, shared by shops, name entry screen and
/// settings menu.
struct MenuCursor {
  struct WindowSprite *anchor; ///< the cursor's sprite, moved with its OAM
  u16 x;       ///< screen pixels: where the current move started
  u16 y;       ///< screen pixels: where the current move started
  s16 targetX; ///< screen pixels
  s16 targetY; ///< screen pixels
  s8 active;   ///< frames of the current move done; reset when it ends
  s8 kind;     ///< frames the move takes; 0 when the cursor is still
};

STATIC_ASSERT(sizeof(struct MenuCursor) == 0x10);
STATIC_ASSERT(offsetof(struct MenuCursor, targetX) == 0x08);
STATIC_ASSERT(offsetof(struct MenuCursor, active) == 0x0c);

/// A menu cursor: its window sprite, or the cursor that approaches its target.
// Screens that write only the image pointer use the union to retain store
// order.
union MenuCursorRef {
  struct WindowSprite *sprite; ///< the image alone; cursor.anchor
  struct MenuCursor cursor;
};

#endif
