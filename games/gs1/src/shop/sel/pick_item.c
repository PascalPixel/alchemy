#include "scene.h"
#include "abi/shop/sel/pick_item.h"
#include "shop.h"

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

s32 Modulo(s32 value, s32 divisor);
void WaitFrames(s32 frames);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWindow_Close(s32 window, s32 style);
struct ShopCursorAnchor *Sys_Run(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);

void Audio_PlayCue(s32 cue);

/* Select a party member and then an item owned by that member. */
s32 Shop_PickUnitItem(s32 *selected_unit, s32 *selected_item)
{
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;
    s32 list_window;
    s32 selected_index = 0;
    s32 redraw = 1;
    s32 unit_id = 0;
    s32 item_slot;
    s32 result = 0;

    Sys_Run2();
    shop = SHOP_RUNTIME;
    shop->item_window = UiWindow_CreateFar(16, 12, 14, 8, 2);
    list_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
    cursor_anchor = Sys_Run(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        result);
    cursor_anchor->kind = 4;
    cursor_anchor->unknown_00[4] = result;
    Sys_Place(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    shop->mode = 12;
    Sys_SetRange(list_window, 2, 0, 8, result);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selected_index = Modulo(
                selected_index + shop->party_member_count,
                shop->party_member_count);
            unit_id = shop->party_member_ids[selected_index];
            Shop_PlaceCursor((void *)list_window, selected_index * 24 - 12, 0);
            shop->mode = 3;
            Shop_DrawParty(list_window, selected_index, 0);
            Shop_DrawUnitGrid(shop->item_window, unit_id);
        }

        WaitFrames(1);
        if ((INPUT_NEW_KEYS & 1) != 0) {
            if (Sys_Check(unit_id) == 0) {
                Audio_PlayCue(0x71);
                continue;
            }

            Audio_PlayCue(0x70);
            item_slot = Sys_Check2(unit_id);
            if (item_slot == -1) {
                shop->cursor.anchor->kind = 4;
                shop->mode = 12;
                redraw = 1;
                continue;
            }
            *selected_unit = unit_id;
            *selected_item = item_slot;
            result = 0;
            goto done;
        }

        if ((INPUT_NEW_KEYS & 2) != 0) {
            Audio_PlayCue(0x71);
            *selected_unit = -1;
            *selected_item = -1;
            result = -1;
            goto done;
        }

        if ((INPUT_REPEAT_KEYS & 0x20) != 0) {
            Audio_PlayCue(0x6f);
            selected_index--;
            redraw = 1;
        }
        if ((INPUT_REPEAT_KEYS & 0x10) != 0) {
            Audio_PlayCue(0x6f);
            selected_index++;
            redraw = 1;
        }
    }

done:
    Sys_Run3();
    UiWindow_Close(list_window, 2);
    UiWindow_Close(shop->item_window, 2);
    WaitFrames(1);
    Sys_Run4();
    return result;
}
