/* Draft, not exact (2026-09-29, Mercury): score 465, only the anchor reload
   differs. OAM attribute bitfields give the ROM's word masks (0xfffffe00,
   0x1ff) and a one-halfword struct keeps the zero a pool constant
   (FAKEMATCH). The ROM reloads cursor->anchor after the attribute store,
   which only a store in alias set 0 forces: a char store does it here, but
   pointer or char members in an attribute union, a char first field and a
   void pointer in the sprite do not. */
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

void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y)
{
    struct ShopCursorSprite *sprite;
    struct Half zero;
    u32 mask;

    sprite = (struct ShopCursorSprite *)cursor->anchor;
    mask = 0xffff;
    zero.v = 0;
    cursor->kind = 1;
    sprite->x = x;
    cursor->target_x = x;
    cursor->x = x;
    cursor->active = zero.v;
    sprite->oam.x = x & mask;
    sprite = (struct ShopCursorSprite *)cursor->anchor;
    cursor->target_y = y;
    cursor->y = y;
    sprite->y = y;
    sprite->oam.y = y & mask;
}
