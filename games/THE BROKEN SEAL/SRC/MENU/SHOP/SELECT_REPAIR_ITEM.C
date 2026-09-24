#include "SHOP.H"
#include "BATTLE_RUNTIME.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"

void UiWork_FinalizeFar(s32, s32);
s32 Inventory_CountFar(s32);
void Audio_PlayCue(s32);
void UiMessage_ShowAndWait(s32);
void Shop_RepairItem(s32, s32);

extern u8 Value_00000075;
extern u8 Value_00000cc2;

/*
 * Repair flow reached from Shop_SelectPartyMember when the shop's party
 * action is not "sell": browse the chosen member's inventory, priced one
 * slot at a time, and hand a confirmed slot off to Shop_RepairItem before
 * showing the repair-result message.
 */
s32 Shop_SelRepair(s32 unit_id)
{
    struct ShopRuntime *shop;
    s32 item_count;
    s32 price_window;
    s32 list_window;
    struct BattleUnit *unit;
    s32 selection;
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
                item_count = Inventory_CountFar(unit_id);
                if (selection > item_count - 1)
                    selection = item_count - 1;
                item_id = 0x1ff & unit->inventory[selection];
                window = (void *)shop->item_window;
                x = Math_Mod(selection, 5) * 16;
                y = Math_Div(selection, 5) * 16 + 8;
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
                goto done;
            }
            if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                result = -1;
                goto done;
            }
            if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                selection -= 1;
                selection = Math_Mod(selection + item_count, item_count);
                redraw = 1;
            }
            if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                selection += 1;
                selection = Math_Mod(selection + item_count, item_count);
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
done:

        UiWork_FinalizeFar(price_window, 2);
        WaitFrames(1);
        if (result != 0)
            break;

        Shop_RepairItem(unit_id, selection);
        UiMessage_ShowAndWait((s32)&Value_00000cc2);
        if (Inventory_CountFar(unit_id) == 0)
            break;
    }

    UiWork_FinalizeFar(list_window, 2);
    return result;
}
