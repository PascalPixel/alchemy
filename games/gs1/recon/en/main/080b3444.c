#include "shop.h"

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

s32 Modulo(s32 value, s32 divisor);
void WaitFrames(s32 frames);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWindow_Close(s32 window, s32 style);
struct ShopCursorAnchor *Func_080150c8(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);
void Func_080a1028(s32 window, s32 column, s32 row, s32 height, s32 flags);
void Func_080a1030(void);
void Func_080b010c(void);
void Func_080b0204(void);
void Func_080b0a20(struct ShopCursor *cursor, s32 target_x, s32 target_y);
s32 Func_080b362c(s32 unit_id);
s32 Func_08077248(s32 unit_id);
void Audio_PlayCue(s32 cue);

#define Shop_SelectPartyMemberItem Func_080b3444

/* Select a party member and then an item owned by that member. */
s32 Shop_SelectPartyMemberItem(s32 *selected_unit, s32 *selected_item)
{
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;
    s32 list_window;
    s32 selected_index = 0;
    s32 redraw = 1;
    s32 unit_id = 0;
    s32 item_slot;
    s32 result = 0;

    Func_080b010c();
    shop = SHOP_RUNTIME;
    shop->item_window = UiWindow_CreateFar(16, 12, 14, 8, 2);
    list_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
    cursor_anchor = Func_080150c8(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        result);
    cursor_anchor->kind = 4;
    cursor_anchor->unknown_00[4] = result;
    Func_080b0a20(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    shop->mode = 12;
    Func_080a1028(list_window, 2, result, 8, result);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selected_index = Modulo(
                selected_index + shop->party_member_count,
                shop->party_member_count);
            unit_id = shop->party_member_ids[selected_index];
            Shop_PlaceCursor((void *)list_window, selected_index * 24 - 12, 0);
            shop->mode = 3;
            Shop_UpdatePartyMemberList(list_window, selected_index, 0);
            Shop_DrawPartyMemberItemGrid(shop->item_window, unit_id);
        }

        WaitFrames(1);
        if ((INPUT_NEW_KEYS & 1) != 0) {
            if (Func_08077248(unit_id) == 0) {
                Audio_PlayCue(0x71);
                continue;
            }

            Audio_PlayCue(0x70);
            item_slot = Func_080b362c(unit_id);
            if (item_slot == -1) {
                shop->cursor.anchor->kind = 4;
                shop->mode = 12;
                redraw = 1;
                continue;
            }
            *selected_unit = unit_id;
            *selected_item = item_slot;
            result = 0;
            break;
        }

        if ((INPUT_NEW_KEYS & 2) != 0) {
            Audio_PlayCue(0x71);
            *selected_unit = -1;
            *selected_item = -1;
            result = -1;
            break;
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

    Func_080a1030();
    UiWindow_Close(list_window, 2);
    UiWindow_Close(shop->item_window, 2);
    WaitFrames(1);
    Func_080b0204();
    return result;
}
