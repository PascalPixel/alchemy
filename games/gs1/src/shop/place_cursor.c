#include "shop.h"
#include "global_cells.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

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
    Shop_SetCursorPosition(
        &shop->cursor,
        cursor_x,
        cursor_y,
        (s8)shop->mode);
}
