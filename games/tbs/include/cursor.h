#ifndef ALCHEMY_CURSOR_H
#define ALCHEMY_CURSOR_H

#include "types.h"
#include "layout_guard.h"
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

LAYOUT_SIZE_GUARD(ShopCursor_Size, struct ShopCursor, 0x10);
LAYOUT_OFFSET_GUARD(ShopCursor_TargetX, struct ShopCursor, targetX, 0x08);
LAYOUT_OFFSET_GUARD(ShopCursor_Active, struct ShopCursor, active, 0x0c);

// Screens that write only the image pointer use the union to retain store
// order.
union MenuCursor {
  struct RenderOutput *output;
  struct ShopCursor cursor;
};

#endif
