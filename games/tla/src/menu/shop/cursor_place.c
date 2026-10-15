#include "types.h"
#include "render_input.h"
#include "ram_buffer.h"

struct ShopCursor {
  struct RenderOutput *anchor;
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
  struct RenderOutput *next;
  u8 unk04[2];
  u16 x;
  u16 y;
  u8 unk0a[0x0a];
  struct SpriteAttr2 oam;
};

void Shop_SetCursor(struct ShopCursor *cursor, s32 targetX, s32 targetY,
                    s8 kind);

void Shop_PlaceCursor(void *window, s32 x, s32 y) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;

  if (window != NULL) {
    x += (s16)((struct RenderInput *)window)->x * 8 + 8;
    y += (s16)((struct RenderInput *)window)->y * 8 + 8;
  }
  Shop_SetCursor(&shop->cursor, x, y, (s8)shop->mode);
}
