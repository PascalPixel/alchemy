#include "SCENE.H"
#include "SHOP.H"
#include "TYPES.H"

void Shop_SetCursor(
    struct ShopCursor *cursor,
    s32 target_x,
    s32 target_y,
    s8 kind)
{
    struct ShopCursorAnchor *anchor = cursor->anchor;

    cursor->x = anchor->x;
    cursor->y = anchor->y;
    cursor->target_x = target_x;
    cursor->target_y = target_y;
    cursor->kind = kind;
    cursor->active = 0;
}

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct SpriteAttr {
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

/* The render output the cursor is anchored to: it links to the next one
   like the anchor, then keeps its position and its OAM attributes. */
struct ShopCursorSprite {
    struct ShopCursorAnchor *next;
    u8 unknown_04[2];
    u16 x;
    u16 y;
    u8 unknown_0a[0x0a];
    struct SpriteAttr oam;
};

/* Places the cursor sprite at (x, y) at once: the position, the target and
   the sprite's OAM coordinates all take the new values. */
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y)
{
    {
        struct ShopCursorSprite *sprite = (struct ShopCursorSprite *)cursor->anchor;

        cursor->target_x = x;
        cursor->kind = 1;
        cursor->active = 0;
        cursor->x = x;
        sprite->oam.x = sprite->x = x;
    }
    {
        struct ShopCursorSprite *sprite;

        cursor->target_y = y;
        cursor->y = y;
        sprite = (struct ShopCursorSprite *)cursor->anchor;
        sprite->oam.y = sprite->y = y;
    }
}
#endif
