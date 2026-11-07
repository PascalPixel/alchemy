#include "types.h"
#include "render_input.h"
#include "ram_buffer.h"

struct ShopCursor {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

struct ShopRuntime {
  u8 unk000[0x4dc];
  struct ShopCursor cursor;
  u8 unk4ec[0x19];
  u8 mode;
};

struct SpriteAttr {
  unsigned y : 8;
  unsigned affine : 2;
  unsigned blendMode : 2;
  unsigned mosaic : 1;
  unsigned fullColor : 1;
  unsigned shape : 2;
  unsigned x : 9;
  unsigned affineIndex : 5;
  unsigned size : 2;
};

struct SpriteAttr2 {
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

struct ShopCursorSprite3 {
  struct WindowSprite *next;
  u8 unk04[2];
  u16 x;
  u16 y;
  u8 unk0a[0x0a];
  struct SpriteAttr2 oam;
};

void Shop_SetCursor(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                    s8 kind);

/// Moves the cursor's sprite a quarter of the way to its target on each
/// axis, at least one pixel.
// The Broken Seal's ShopCursor_MoveTowardTarget.
void Func_081089e4(struct ShopCursor *cursor) {
  struct ShopCursorSprite3 *sprite;
  s32 delta;
  s32 step;
  u32 mask;

  sprite = (struct ShopCursorSprite3 *)cursor->anchor;
  if (sprite != NULL) {
    delta = sprite->x - cursor->targetX;
    step = delta / 4;
    if (step < 0)
      step = -step;
    if (delta > 0) {
      if (step != 0)
        sprite->x = sprite->x - step;
      else
        sprite->x = sprite->x - 1;
    } else if (delta < 0) {
      if (step != 0)
        sprite->x = sprite->x + step;
      else
        sprite->x = sprite->x + 1;
    } else {
      // Neither move: skip the shared sprite update.
      goto y_axis;
    }
    {
      /* FAKEMATCH: the barrier reloads x before the 0x1ff mask is built, and x
       * pinned to r3 leaves the mask r2, as native; plain, as The Broken Seal
       * writes it in each branch, 2 instructions differ in tla-en. */
      register u32 x asm("r3") = sprite->x;

      asm volatile("");
      mask = 0x1ff;
      mask &= x;
      sprite->oam.x = mask;
    }

  y_axis:
    delta = sprite->y - cursor->targetY;
    step = delta / 4;
    if (step < 0)
      step = -step;
    if (delta > 0) {
      if (step != 0)
        sprite->y = sprite->y - step;
      else
        sprite->y = sprite->y - 1;
      sprite->oam.y = sprite->y;
    } else if (delta < 0) {
      if (step != 0)
        sprite->y = sprite->y + step;
      else
        sprite->y = sprite->y + 1;
      sprite->oam.y = sprite->y;
    }
  }
}

void Shop_SetCursor(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                    s8 kind) {
  struct WindowSprite *anchor = cursor->anchor;

  cursor->x = (u16)anchor->x;
  cursor->y = (u16)anchor->y;
  cursor->targetX = targetX;
  cursor->targetY = targetY;
  cursor->kind = kind;
  cursor->active = 0;
}

/// Puts the cursor at x, y at once: both its position and its target, and its
/// sprite with them.
// The Broken Seal's ShopCursor_SetPositionImmediate.
void Func_08108aa8(struct ShopCursor *cursor, s32 x, s32 y) {
  {
    struct ShopCursorSprite3 *sprite =
        (struct ShopCursorSprite3 *)cursor->anchor;

    cursor->targetX = x;
    cursor->kind = 1;
    cursor->active = 0;
    cursor->x = x;
    sprite->oam.x = sprite->x = x;
  }
  {
    struct ShopCursorSprite3 *sprite;

    cursor->targetY = y;
    /* FAKEMATCH: the volatile store of y and reload of the anchor keep the
     * reload after the store, as native; plain, the reload is scheduled ahead
     * of it (one reordered pair, differ 2). */
    *(volatile u16 *)&cursor->y = y;
    sprite = *(struct ShopCursorSprite3 *volatile *)&cursor->anchor;
    sprite->oam.y = sprite->y = y;
  }
}
