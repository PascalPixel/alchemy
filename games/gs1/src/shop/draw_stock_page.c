#include "shop.h"

s32 Func_080022fc(s32 value, s32 divisor);
void Func_08015060(s32 window);
u8 *Func_080150c8(u16 no, u32 flags, s32 window, s32 x, s32 y);
u8 *Func_080152d0(s32 no, s32 kind, s32 window, s32 x, s32 y);
u8 *Func_080b0744(s16 value, s32 window, s32 x, s32 y);

void Shop_DrawStockPage(s32 window, s32 selected)
{
    struct ShopRuntime *shop;
    s16 *item_ids;
    s16 *item_id;
    s32 item_count;
    s32 first;
    s32 slot;
    s32 x;
    s32 clear;
    s32 highlight;
    s16 stock_item;
    struct ItemDefinition *definition;
    u8 *icon;

    shop = SHOP_RUNTIME;
    item_ids = shop->stock_item_ids;
    item_count = shop->stock_count;
    first = selected - Func_080022fc(selected, 7);
    if (window != 0) {
        Func_08015060(window);
        if (first != 0) {
            icon = Func_080150c8(shop->previous_page_icon, 0x40000000,
                                 window, 216, -16);
            clear = 0;
            icon[4] = clear;
            icon[5] = 17;
            *(u16 *)(icon + 12) = clear;
        }
        if (first + 7 < item_count) {
            icon = Func_080150c8(shop->next_page_icon, 0x40000000,
                                 window, 216, 24);
            clear = 0;
            icon[4] = clear;
            icon[5] = 15;
            *(u16 *)(icon + 12) = clear;
        }
        slot = 0;
        if ((u32)first < (u32)item_count) {
            item_id = (s16 *)((u32)(first * 2) + (u32)item_ids);
            x = 16;
            for (; (u32)slot <= 6 && (u32)first < (u32)item_count;
                 x += 32, slot++, item_id++, first++) {
                stock_item = *item_id;
                definition = Item_Get(stock_item);
                icon = Func_080152d0(
                    stock_item, 1, window, slot * 32, 0);
                icon[15] = 252;
                if (first == selected) {
                    icon[5] = 9;
                    highlight = 10;
                    *(u16 *)(icon + 12) = highlight;
                    icon[15] = 253;
                }
                icon = Func_080b0744(definition->price, window, x, 0);
                icon[15] = 251;
            }
        }
    }
}
