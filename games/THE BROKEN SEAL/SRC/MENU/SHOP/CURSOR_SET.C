#include "SCENE.H"
#include "SHOP.H"

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
