#include "SHOP.H"
#include "UI.H"
#include "PARTY_STATE.H"
#include "IO_REG.H"
#include "SOUND_IDS.H"
extern struct ShopRuntime *gMenuWork;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

/* main:080b0aac Shop_SelBuy - matches (alchemy diff 0 of 531), no devices.

   2026-10-05 sun deep finisher 17: what closed the 356 left by earlier
   waves was the source shape, not devices:
   - the old cursor is its own local (prev), not the pick result; prev is
     live only across the wrap and takes r8 early, and the result shares r8
     with it once it conflicts with the cursor;
   - the party cursor is reset to 0 with the other party locals before the
     party window opens, so it is live while the stock result is, keeping
     the result out of r7; member = 0 comes before ask_again = 0, so the
     flag copies the member's register;
   - both loops leave by goto to a label right after the loop (as
     Shop_SelSell does); the loop optimiser moves those exit blocks behind
     the page loop's goto, giving the ROM's order at the end;
   - the stock id is read through a pointer to the id list, which keeps
     shop+0x26c as its own address register;
   - "buy anyway?" sets ask_again only on the refusal path, so the buy code
     after it is a join CSE cannot skip into, and &selected_item is formed
     again there as in the ROM;
   - the menu's zero arguments are written as 0; CSE uses the result's
     register for them as the ROM does.
   The page loop is a goto loop: as for (;;) the loop optimiser hoists
   &shop->mode out of it (413). */

extern u8 MsgItemPlainName;

struct ItemDefinition *Item_Get(s32 item);
void UiWork_FinalizeFar(s32 window, s32 style);
void RenderOutput_RedrawSavedRectFar(s32 window);
void UiMessage_ShowAndWait(s32 message);
s32 UiMessage_ShowChoice(s32 value);
void UiMessage_ShowAndRestoreState(s32 message);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void PsynergyMenu_InitializeEntryObjectsFar(s32 window, s32 column, s32 row, s32 height, s32 flags);
void Menu_ReleaseEntryObjectsFar(void);
s32 Item_GetEquipmentGroupFar(s32 item);
void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id);
s32 Inventory_AddItemFar(s32 unit_id, s32 item_id);
void Inventory_RemoveFar(s32 unit_id, s32 slot);
s32 Inventory_CountFar(s32 unit_id);
s32 Item_IsCompatibleWithOwnerFar(s32 unit_id, s32 item_id);
void Shop_BuySpecialItem(s32 window, s32 item_window);
void Item_AdjustCounterFar(s32 item, s32 delta);
s32 AbilityMenu_BuildAvailableList(void);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);

s32 Shop_SelBuy(void)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 price_window = 0;
    s32 stock_window;
    s32 message_window;
    s32 party_window;
    s32 selected = 0;
    s32 quantity;
    struct ItemDefinition *item;
    s32 ask_again;
    s32 item_window;
    s32 redraw;
    s32 cursor;
    s32 count;
    s32 item_id;
    struct ItemDefinition *stock_item;
    s32 result;
    s32 prev;
    s32 member;
    s32 slot;
    s32 i;
    s16 *ids;

    shop->item_window = 0;
    shop->money_window = UiWindow_CreateFar(18, 7, 12, 4, 2);
    Shop_DrawMoney();
    price_window = UiWindow_CreateFar(0, 8, 15, 4, 2);

next_page:
    cursor = selected;
    stock_window = UiWindow_CreateFar(0, 12, 30, 4, 2);
    shop->cursor.anchor->active = 18;
    shop->mode = 12;
    message_window = UiWindow_CreateFar(0, 17, 30, 3, 2);
    redraw = 1;

    for (;;) {
        count = shop->stock_count;
        if (redraw != 0) {
            ids = shop->stock_item_ids;
            item_id = ids[cursor];
            stock_item = Item_Get(item_id);
            redraw = 0;
            Shop_PlaceCursor((void *)stock_window, cursor % 7 * 32 - 8, 8);
            shop->mode = 4;
            Shop_DrawStock(stock_window, cursor);
            Shop_DrawMsg(message_window, item_id + (s32)&MsgItemPlainName);
            RenderOutput_RedrawSavedRectFar(price_window);
            Shop_DrawItemPrice(price_window, item_id, stock_item->price, 0);
        }
        if ((gKeyState & KEY_A) != 0) {
            selected = cursor;
            Audio_PlayCue(SOUND_MENU_CONFIRM);
            result = 0;
            goto stock_done;
        }
        if ((gKeyState & KEY_B) != 0) {
            Audio_PlayCue(SOUND_MENU_CANCEL);
            result = -1;
            goto stock_done;
        }
        if ((gKeysRepeat & KEY_LEFT) != 0) {
            prev = cursor;
            cursor--;
            cursor = (cursor + count) % count;
            if (prev != cursor) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
            }
        }
        if ((gKeysRepeat & KEY_RIGHT) != 0) {
            prev = cursor;
            cursor++;
            cursor = (cursor + count) % count;
            if (prev != cursor) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
            }
        }
        if ((gKeysRepeat & KEY_UP) != 0 && cursor - 7 >= 0) {
            cursor -= 7;
            redraw = 1;
        }
        if ((gKeysRepeat & KEY_DOWN) != 0) {
            if (cursor + 7 < (count + 6) / 7 * 7) {
                cursor += 7;
                redraw = 1;
            }
            if (cursor > count - 1)
                cursor = count - 1;
        }
        WaitFrames(1);
    }

stock_done:
    UiWork_FinalizeFar(message_window, 2);
    UiWork_FinalizeFar(stock_window, 2);
    WaitFrames(1);
    if (result != 0)
        goto close;

    shop->selected_item = shop->stock_item_ids[selected];
    UiMessage_ShowAndWait(0xc9d);
    item = Item_Get(shop->selected_item);
    quantity = 1;
    member = 0;
    ask_again = 0;
    cursor = 0;
    party_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
    shop->cursor.anchor->active = 4;
    shop->mode = 12;
    PsynergyMenu_InitializeEntryObjectsFar(party_window, 2, 0, 8, 0);
    item_window = UiWindow_CreateFar(16, 11, 14, 9, 2);
    redraw = 1;

    for (;;) {
        if (ask_again != 0) {
            ask_again = 0;
            UiMessage_ShowAndWait(0xc9d);
            redraw = 1;
        }
        if (redraw != 0) {
            redraw = 0;
            cursor = (cursor + shop->party_member_count) % shop->party_member_count;
            member = shop->party_member_ids[cursor];
            Shop_PlaceCursor((void *)party_window, cursor * 24 - 12, 0);
            shop->mode = 3;
            Shop_DrawParty(party_window, cursor, shop->selected_item);
            if (Item_GetEquipmentGroupFar(shop->selected_item) == 0)
                Shop_DrawUnitItem(item_window, member, shop->selected_item);
            else
                Shop_DrawEquipComparison(item_window, member, shop->selected_item);
        }
        if ((gKeyState & KEY_A) != 0) {
            slot = Inventory_AddItemFar(member, shop->selected_item);
            if (slot < 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                UiWork_PushValueSlotFar(member, 1);
                UiWork_PushValueSlotFar(shop->selected_item, 2);
                if (Inventory_CountFar(member) == 15)
                    UiMessage_ShowAndWait(0xc9e);
                else
                    UiMessage_ShowAndWait(0xca6);
                continue;
            }
            Inventory_RemoveFar(member, slot);
            if ((u32)item->price > gGameState.coins) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                UiMessage_ShowAndRestoreState(0xc9c);
                result = -1;
                goto party_done;
            }
            if (Item_IsCompatibleWithOwnerFar(member, shop->selected_item) == 0) {
                UiWork_PushValueSlotFar(member, 1);
                UiMessage_ShowAndWait(0xc9f);
                if (UiMessage_ShowChoice(0) != 0) {
                    ask_again = 1;
                    continue;
                }
            }
            Audio_PlayCue(SOUND_MENU_CONFIRM);
            WaitFrames(1);
            quantity = Shop_SelBuyNum(member, shop->selected_item);
            ask_again = 1;
            if (quantity == -1)
                continue;
            Shop_BuyDone(member, shop->selected_item, quantity);
            Shop_BuySpecialItem(party_window, item_window);
            result = 0;
            goto party_done;
        }
        if ((gKeyState & KEY_B) != 0) {
            Audio_PlayCue(SOUND_MENU_CANCEL);
            result = -1;
            goto party_done;
        }
        if ((gKeysRepeat & KEY_LEFT) != 0) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            cursor--;
            redraw = 1;
        }
        if ((gKeysRepeat & KEY_RIGHT) != 0) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            cursor++;
            redraw = 1;
        }
        WaitFrames(1);
    }

party_done:
    Menu_ReleaseEntryObjectsFar();
    UiWork_FinalizeFar(item_window, 2);
    UiWork_FinalizeFar(party_window, 2);
    WaitFrames(1);
    if (result == 0 && shop->party_action == 2) {
        for (i = 0; i < quantity; i++)
            Item_AdjustCounterFar(shop->selected_item, -1);
        if (AbilityMenu_BuildAvailableList() == 0)
            goto close;
        if (selected > shop->stock_count - 1)
            selected = shop->stock_count - 1;
    }
    UiMessage_ShowAndWait(0xca8);
    goto next_page;

close:
    UiWork_FinalizeFar(price_window, 2);
    UiWork_FinalizeFar(shop->money_window, 2);
    WaitFrames(1);
    return 0;
}
