#include "SCENE.H"
#include "SHOP.H"
#include "TYPES.H"

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

extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];

/* shop/place_cursor.c */
void Shop_SetCursor(struct ShopCursor *cursor, s32 x, s32 y, s8 kind);

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

void Shop_PlaceCursor(void *window, s32 x, s32 y)
{
    s32 cursor_x;
    s32 cursor_y;
    struct ShopRuntime *shop;

    cursor_x = x;
    cursor_y = y;
    shop = gMenuWork;
    if (window != NULL) {
        cursor_x = cursor_x + (FIELD_AT_OFFSET(window, u16 *, 0xC) * 8) + 8;
        cursor_y = cursor_y + (FIELD_AT_OFFSET(window, u16 *, 0xE) * 8) + 8;
    }
    Shop_SetCursor(
        &shop->cursor,
        cursor_x,
        cursor_y,
        (s8)shop->mode);
}
