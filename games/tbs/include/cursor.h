#ifndef ALCHEMY_CURSOR_H
#define ALCHEMY_CURSOR_H

#include "types.h"
#include "static_assert.h"
#include "render_input.h"

// Cursor that approaches its target, shared by shop, name entry and settings.
struct ShopCursor {
  struct RenderOutput *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

STATIC_ASSERT(sizeof(struct ShopCursor) == 0x10);
STATIC_ASSERT(offsetof(struct ShopCursor, targetX) == 0x08);
STATIC_ASSERT(offsetof(struct ShopCursor, active) == 0x0c);

// Screens that write only the image pointer use the union to retain store
// order.
union MenuCursor {
  struct RenderOutput *output;
  struct ShopCursor cursor;
};

#endif
