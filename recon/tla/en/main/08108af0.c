/*
 * Draft: Shop_PlaceCursor over ⚓️'s menu work (heap slot 0xdc; cursor at
 * 0x4dc, mode at 0x505) with signed window coordinates. Score 2: adds r1, #8
 * is issued after the y load in the listing. The rest is load scheduling: the
 * native issues loads (and fills their delay) earlier than agscc at
 * -mcpu=arm7tdmi; with -mtune=arm9tdmi added it scores 0.
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

void Shop_PlaceCursor(void *window, s32 x, s32 y)
{
    struct ShopRuntime *shop = Ram_HeapSlots->menu_runtime;

    if (window != NULL) {
        x += (s16)((struct RenderInput *)window)->x * 8 + 8;
        y += (s16)((struct RenderInput *)window)->y * 8 + 8;
    }
    Shop_SetCursor(
        &shop->cursor,
        x,
        y,
        (s8)shop->mode);
}
