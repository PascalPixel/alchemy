#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "shop.h"
#include "fixed_math.h"
void RenderOutput_PrepareForRedrawFar(s32 window);
struct ItemDefinition *Item_GetDirectFar(s32 item);
struct RenderOutput *RenderOutput_CreateLoadedFar(s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y);

#define RenderOutput_CreateLoadedFar ((struct RenderOutput * (*)(s32,s32,s32,s32,s32))RenderOutput_CreateLoadedFar)


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
        RenderOutput_PrepareForRedrawFar(window);
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
                definition = Item_GetDirectFar(stock_item);
                icon = RenderOutput_CreateLoadedFar(
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
