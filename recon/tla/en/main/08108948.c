#include "types.h"
#include "render_input.h"
#include "ram_buffer.h"

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
    struct ShopCursor cursor;
    u8 unknown_4ec[0x19];
    u8 mode;
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

void ShopCursor_Advance(struct ShopCursor *cursor)
{
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
    x = cursor->x + (cursor->target_x - (s16)cursor->x) * step / kind;
    sprite->x = x;
    ((struct SpriteAttr *)&sprite->packed)->x = x;
    y = cursor->y + (cursor->target_y - (s16)cursor->y) * step / kind;
    sprite->y = y;
    ((struct SpriteAttr *)&sprite->packed)->y = y;
    if (step == kind) {
        cursor->kind = 0;
        cursor->active = 0;
    }
}
