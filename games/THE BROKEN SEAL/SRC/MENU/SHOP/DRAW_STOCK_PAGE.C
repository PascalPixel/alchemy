#include "PROTO.H"
#include "SHOP.H"
#include "FIXED_MATH.H"

#define UiIcon_Draw ((struct RenderOutput * (*)(s32,s32,s32,s32,s32))UiIcon_Draw)
extern struct ShopRuntime *gMenuWork;


void Shop_DrawStock(s32 window, s32 selected)
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
    struct RenderOutput *icon;

    shop = gMenuWork;
    item_ids = shop->stock_item_ids;
    item_count = shop->stock_count;
    first = selected - selected % 7;
    if (window != 0) {
        UiWindow_Clear(window);
        if (first != 0) {
            icon = RenderOutput_CreateFar(shop->previous_page_icon, 0x40000000,
                                 (struct RenderInput *)window, 216, -16);
            clear = 0;
            icon->kind = clear;
            icon->active = 17;
            icon->anim_step = clear;
        }
        if (first + 7 < item_count) {
            icon = RenderOutput_CreateFar(shop->next_page_icon, 0x40000000,
                                 (struct RenderInput *)window, 216, 24);
            clear = 0;
            icon->kind = clear;
            icon->active = 15;
            icon->anim_step = clear;
        }
        slot = 0;
        if (first < (u32)item_count) {
            item_id = (s16 *)((first * 2) + (u32)item_ids);
            x = 16;
            for (; (u32)slot <= 6 && first < (u32)item_count;
                 x += 32, slot++, item_id++, first++) {
                stock_item = *item_id;
                definition = Item_Get(stock_item);
                icon = UiIcon_Draw(
                    stock_item, 1, window, slot * 32, 0);
                icon->sentinel = 252;
                if (first == selected) {
                    icon->active = 9;
                    highlight = 10;
                    icon->anim_step = highlight;
                    icon->sentinel = 253;
                }
                icon = Shop_CreatePriceSprite(definition->price, window, x, 0);
                icon->sentinel = 251;
            }
        }
    }
}
