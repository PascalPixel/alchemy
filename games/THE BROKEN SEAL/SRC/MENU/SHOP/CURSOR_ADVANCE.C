/* ShopCursor_Advance: step the cursor sprite one frame of a linear tween
   from its position toward its target over kind frames, refreshing the
   sprite's position and OAM coordinates, and stop when the tween ends.
   FAKEMATCH: the zero that ends the tween is a one-halfword struct, which
   keeps it a pool constant loaded before the second division as in the ROM. */
#include "TYPES.H"
#include "SHOP.H"

struct SpriteAttr {
    unsigned y : 8;
    unsigned affine : 2;
    unsigned blend_mode : 2;
    unsigned mosaic : 1;
    unsigned full_color : 1;
    unsigned shape : 2;
    unsigned x : 9;
    unsigned affine_index : 5;
    unsigned size : 2;
};

struct ShopCursorSprite {
    u8 unknown_00[6];
    u16 x;
    u16 y;
    u8 unknown_0a[0x0a];
    struct SpriteAttr oam;
};

struct Half {
    u16 v;
};

void ShopCursor_Advance(struct ShopCursor *cursor)
{
    struct ShopCursorSprite *sprite;
    s32 kind;
    s32 step;
    s32 x;
    s32 y;
    struct Half zero;

    if (cursor == NULL)
        return;
    kind = cursor->kind;
    if (kind == 0)
        return;
    sprite = (struct ShopCursorSprite *)cursor->anchor;
    step = (s8)++cursor->active;
    x = cursor->x + (cursor->target_x - (s16)cursor->x) * step / kind;
    sprite->x = x;
    zero.v = 0;
    sprite->oam.x = x;
    y = cursor->y + (cursor->target_y - (s16)cursor->y) * step / kind;
    sprite->y = y;
    sprite->oam.y = y;
    if (step == kind) {
        cursor->kind = zero.v;
        cursor->active = zero.v;
    }
}
