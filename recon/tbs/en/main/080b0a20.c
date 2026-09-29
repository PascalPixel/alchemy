/* Draft, not exact (2026-09-29, Mercury): the code is exact; only the
   literal pool order differs. The ROM pools 0xffff, 0, 0x1ff, 0xfffffe00;
   this emits 0, 0x1ff, 0xffff, 0xfffffe00. The one-halfword zero and the
   bitfield mask are pooled when their loads are expanded, the u32 mask
   only at reload; a one-halfword or one-word mask, an early x & mask and
   either statement order keep 0xffff behind them. A volatile view of
   the anchor rereads it for the y half, as the ROM does (FAKEMATCH). */
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
    {
        struct ShopCursorSprite *again;

        again = *(struct ShopCursorSprite * volatile *)&cursor->anchor;
        cursor->target_y = y;
        cursor->y = y;
        again->y = y;
        again->oam.y = y & mask;
    }
}
