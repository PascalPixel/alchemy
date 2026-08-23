#include "shop.h"

extern u8 Data_00000000[];

void Shop_SetCursorPosition(
    struct ShopCursor *cursor,
    s32 target_x,
    s32 target_y,
    s8 kind)
{
    struct ShopCursorAnchor *anchor = cursor->anchor;
    s8 active = (s8)(s32)Data_00000000;

    cursor->x = anchor->x;
    cursor->y = anchor->y;
    cursor->target_x = target_x;
    cursor->target_y = target_y;
    cursor->kind = kind;
    cursor->active = active;
}
