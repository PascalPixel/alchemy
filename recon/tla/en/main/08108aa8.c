/*
 * Draft: ShopCursor_SetPositionImmediate, ☀️'s SHOP.C body. Score 4: the
 * target_y store and the anchor reload swap places. The rest is load
 * scheduling: the native issues loads (and fills their delay) earlier than
 * agscc at -mcpu=arm7tdmi; with -mtune=arm9tdmi added it scores 2.
 * With -mtune=arm9tdmi (2026-10-05) score 2: the anchor reload comes before
 * the cursor's y store where the listing stores first. A barrier after the
 * store gives 6; loading the anchor before the store changes nothing.
 */
#include "TYPES.H"
#include "RENDER_INPUT.H"
#include "RAM_BUFFER.H"

struct ShopCursor {
    struct RenderOutput *anchor;
    u16 x;
    u16 y;
    s16 target_x;
    s16 target_y;
    s8 active;
    s8 kind;
};

struct ShopRuntime {
    u8 unknown_000[0x4dc];
    struct ShopCursor cursor;           /* 0x4dc */
    u8 unknown_4ec[0x19];
    u8 mode;                            /* 0x505 */
};

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

struct SpriteAttr2 {
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

struct ShopCursorSprite3 {
    struct RenderOutput *next;
    u8 unknown_04[2];
    u16 x;
    u16 y;
    u8 unknown_0a[0x0a];
    struct SpriteAttr2 oam;
};

void Shop_SetCursor(struct ShopCursor *cursor, s32 target_x, s32 target_y, s8 kind);

/* Places the cursor sprite at (x, y) at once: the position, the target and
   the sprite's OAM coordinates all take the new values. */
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y)
{
    {
        struct ShopCursorSprite3 *sprite = (struct ShopCursorSprite3 *)cursor->anchor;

        cursor->target_x = x;
        cursor->kind = 1;
        cursor->active = 0;
        cursor->x = x;
        sprite->oam.x = sprite->x = x;
    }
    {
        struct ShopCursorSprite3 *sprite;

        cursor->target_y = y;
        cursor->y = y;
        sprite = (struct ShopCursorSprite3 *)cursor->anchor;
        sprite->oam.y = sprite->y = y;
    }
}
