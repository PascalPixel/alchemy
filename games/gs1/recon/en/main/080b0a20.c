#include "types.h"
#include "shop.h"

#define ShopCursor_SetPositionImmediate Func_080b0a20

struct ShopCursorSprite {
    u8 unknown_00[6];
    u16 x;
    u16 y;
    u8 unknown_0a[0x0a];
    u8 screen_y;
    u8 unknown_1b;
    u16 attributes;
};

extern u8 Data_00000000[];

void ShopCursor_SetPositionImmediate(
    struct ShopCursor *cursor,
    s32 x,
    s32 y)
{
    struct ShopCursorSprite *sprite;
    u32 coordinate_mask;
    s8 active;
    u32 screen_x;
    u32 attributes;

    sprite = (struct ShopCursorSprite *)cursor->anchor;
    coordinate_mask = 0xffff;
    active = (s8)(s32)Data_00000000;
    cursor->kind = 1;
    sprite->x = x;
    cursor->target_x = x;
    cursor->x = x;
    screen_x = x & coordinate_mask;
    screen_x &= 0x1ff;
    cursor->active = active;
    attributes = sprite->attributes & ~0x1ff;
    sprite->attributes = screen_x | attributes;

    sprite = (struct ShopCursorSprite *)cursor->anchor;
    cursor->target_y = y;
    cursor->y = y;
    sprite->y = y;
    y &= coordinate_mask;
    sprite->screen_y = y;
}
