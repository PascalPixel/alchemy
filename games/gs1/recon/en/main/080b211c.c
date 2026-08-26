#include "shop.h"
#include "battle_runtime.h"
#include "sound_ids.h"

s32 Modulo(s32, s32);
s32 FixedPoint_Ratio(s32, s32);
void WaitFrames(s32);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWindow_Close(s32, s32);
s32 Ability_GetAvailability(s32);
void Audio_PlayCue(s32);
void UiMessage_ShowAndWait(s32);
void Func_080b2328(s32, s32);

extern u8 Value_00000075;
extern u8 Value_00000cc2;

/*
 * Repair flow reached from Shop_SelectPartyMember when the shop's party
 * action is not "sell": browse the chosen member's inventory, priced one
 * slot at a time, and hand a confirmed slot off to Func_080b2328 before
 * showing the repair-result message.
 */
s32 Func_080b211c(s32 unit_id)
{
    struct ShopRuntime *shop;
    struct BattleUnit *unit;
    s32 list_window;
    s32 price_window;
    s32 selection;
    s32 item_count;
    s32 redraw;
    s32 result;
    s32 item_id;
    s32 price;
    void *window;
    s32 x;
    s32 y;

    shop = SHOP_RUNTIME;
    unit = BattleUnit_Get(unit_id);
    item_count = 1;
    list_window = UiWindow_CreateFar(15, 8, 15, 4, 2);
    selection = 0;

    for (;;) {
        price_window = UiWindow_CreateFar(0, 5, 30, 3, 2);
        shop->cursor.anchor->kind = 18;
        shop->mode = 12;
        redraw = 1;

        for (;;) {
            if (redraw != 0) {
                redraw = 0;
                item_count = Ability_GetAvailability(unit_id);
                if (selection > item_count - 1)
                    selection = item_count - 1;
                item_id = 0x1ff & unit->inventory[selection];
                window = (void *)shop->item_window;
                x = Modulo(selection, 5) * 16;
                y = FixedPoint_Ratio(selection, 5) * 16 + 8;
                Shop_PlaceCursor(window, x, y);
                shop->mode = 3;
                price = Shop_ComputeRepairPrice(unit->inventory[selection]);
                Shop_DrawItemPrice(list_window, item_id, price, 2);
                Shop_DrawMessage(
                    price_window, item_id + (s32)&Value_00000075);
            }

            if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
                result = 0;
                break;
            }
            if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                result = -1;
                break;
            }
            if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                selection = Modulo(selection - 1 + item_count, item_count);
                redraw = 1;
            }
            if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                selection = Modulo(selection + 1 + item_count, item_count);
                redraw = 1;
            }
            if ((*(volatile u32 *)ADDR_03001B04 & 0x40) != 0) {
                selection -= 5;
                if (selection < 0)
                    selection += 15;
                while (selection >= item_count)
                    selection -= 5;
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
            }
            if ((*(volatile u32 *)ADDR_03001B04 & 0x80) != 0) {
                selection += 5;
                if (selection >= item_count)
                    selection -= 15;
                while (selection < 0)
                    selection += 5;
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
            }
            WaitFrames(1);
        }

        UiWindow_Close(price_window, 2);
        WaitFrames(1);
        if (result != 0)
            break;

        Func_080b2328(unit_id, selection);
        UiMessage_ShowAndWait((s32)&Value_00000cc2);
        if (Ability_GetAvailability(unit_id) == 0)
            break;
    }

    UiWindow_Close(list_window, 2);
    return result;
}
