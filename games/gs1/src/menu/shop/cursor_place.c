#include "types.h"
#include "scene.h"
#include "shop.h"

/* shop/place_cursor.c */
void Shop_SetCursor(struct ShopCursor *cursor, s32 x, s32 y, s8 kind);

void Shop_PlaceCursor(void *window, s32 x, s32 y)
{
    s32 cursor_x;
    s32 cursor_y;
    struct ShopRuntime *shop;

    cursor_x = x;
    cursor_y = y;
    shop = SHOP_RUNTIME;
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
