// Near miss, differ 16 of 71 (draft 69): moves the shop cursor one step of
// its glide toward the target, setting the sprite's position and its OAM x
// and y, and stops the glide on its last step. Other: the listing builds the
// x field's 0x1ff mask with split constants where this loads it from the pool
// (writing the OAM through the RenderOutput's oam bitfields directly gives
// the same differ), and the step's increment and the start x load come after
// the target x load there.
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

void ShopCursor_Advance(struct ShopCursor *cursor) {
  struct RenderOutput *sprite;
  s32 kind;
  s32 step;
  s32 x;
  s32 y;

  if (cursor == NULL)
    return;
  kind = cursor->kind;
  if (kind == 0)
    return;
  sprite = cursor->anchor;
  step = (s8)++cursor->active;
  x = cursor->x + (cursor->targetX - (s16)cursor->x) * step / kind;
  sprite->x = x;
  ((struct SpriteAttr *)&sprite->oam)->x = x;
  y = cursor->y + (cursor->targetY - (s16)cursor->y) * step / kind;
  sprite->y = y;
  ((struct SpriteAttr *)&sprite->oam)->y = y;
  if (step == kind) {
    cursor->kind = 0;
    cursor->active = 0;
  }
}
