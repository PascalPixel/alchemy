#include "scene.h"
#include "shop.h"

extern u8 gVal[];

void Shop_SetCursor(
    struct ShopCursor *cursor,
    s32 target_x,
    s32 target_y,
    s8 kind)
{
    struct ShopCursorAnchor *anchor = cursor->anchor;
    s8 active = (s8)(s32)gVal;

    cursor->x = anchor->x;
    cursor->y = anchor->y;
    cursor->target_x = target_x;
    cursor->target_y = target_y;
    cursor->kind = kind;
    cursor->active = active;
}
