#include "SHOP.H"
#include "UI.H"

/* main:080b0aac Shop_SelBuy - hand-written draft, 523 of 636 halfwords
   differ (1256 of 1272 bytes). The control flow matches the ROM: real
   loops for the stock and member pickers with their exits as labelled
   blocks at the end. Left: register and spill-slot roles (the ROM spills
   the member window and keeps the result in r8), the stock-id address
   association, and the ROM's mid-function literal pool.

   The shop's buy menu: browse the stock seven to a row, then pick the
   member who carries the chosen item (a full bag or an item they cannot
   use asks again), choose how many, and let the special-item offer follow
   the purchase. Artifact shops drop the bought stock and close when it
   runs out. */

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

extern u8 MsgItemPlainName;

s32 Math_Mod(s32 value, s32 divisor);
s32 Math_Div(s32 value, s32 divisor);
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
void Func_080b1260(s32 window, s32 unit_id, s32 item_id);
s32 Inventory_AddItemFar(s32 unit_id, s32 item_id);
void Inventory_RemoveFar(s32 unit_id, s32 slot);
s32 Inventory_CountFar(s32 unit_id);
s32 Item_IsCompatibleWithOwnerFar(s32 unit_id, s32 item_id);
void Shop_BuySpecialItem(s32 window, s32 item_window);
void Func_08077240(s32 item, s32 delta);
s32 AbilityMenu_BuildAvailableList(void);
void Audio_PlayCue(s32 cue);
void WaitFrames(s32 frames);

s32 Shop_SelBuy(void)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
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
    s32 member;
    s32 slot;
    s32 i;

    shop->item_window = 0;
    shop->money_window = UiWindow_CreateFar(18, 7, 12, 4, 2);
    Shop_DrawMoney();
    price_window = UiWindow_CreateFar(0, 8, 15, 4, 2);

outer:
    {
        cursor = selected;
        stock_window = UiWindow_CreateFar(0, 12, 30, 4, 2);
        shop->cursor.anchor->kind = 18;
        shop->mode = 12;
        message_window = UiWindow_CreateFar(0, 17, 30, 3, 2);
        redraw = 1;

        for (;;) {
            count = shop->stock_count;
            if (redraw != 0) {
                item_id = shop->stock_item_ids[cursor];
                stock_item = Item_Get(item_id);
                redraw = 0;
                Shop_PlaceCursor((void *)stock_window, Math_Mod(cursor, 7) * 32 - 8, 8);
                shop->mode = 4;
                Shop_DrawStockPage(stock_window, cursor);
                Shop_DrawMessage(message_window, item_id + (s32)&MsgItemPlainName);
                RenderOutput_RedrawSavedRectFar(price_window);
                Shop_DrawItemPrice(price_window, item_id, stock_item->price, 0);
            }
            if ((INPUT_NEW_KEYS & 1) != 0)
                goto stock_chosen;
            if ((INPUT_NEW_KEYS & 2) != 0)
                goto stock_cancelled;
            if ((INPUT_REPEAT_KEYS & 0x20) != 0) {
                result = cursor;
                cursor = Math_Mod(--cursor + count, count);
                if (result != cursor) {
                    Audio_PlayCue(0x6f);
                    redraw = 1;
                }
            }
            if ((INPUT_REPEAT_KEYS & 0x10) != 0) {
                result = cursor;
                cursor = Math_Mod(++cursor + count, count);
                if (result != cursor) {
                    Audio_PlayCue(0x6f);
                    redraw = 1;
                }
            }
            if ((INPUT_REPEAT_KEYS & 0x40) != 0 && cursor - 7 >= 0) {
                cursor -= 7;
                redraw = 1;
            }
            if ((INPUT_REPEAT_KEYS & 0x80) != 0) {
                if (cursor + 7 < Math_Div(count + 6, 7) * 7) {
                    cursor += 7;
                    redraw = 1;
                }
                if (cursor > count - 1)
                    cursor = count - 1;
            }
            WaitFrames(1);
        }

    close_stock:
        UiWork_FinalizeFar(message_window, 2);
        UiWork_FinalizeFar(stock_window, 2);
        WaitFrames(1);
        if (result != 0)
            goto leave;

        shop->selected_item = shop->stock_item_ids[selected];
        UiMessage_ShowAndWait(0xc9d);
        item = Item_Get(shop->selected_item);
        quantity = 1;
        ask_again = 0;
        member = 0;
        party_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
        shop->cursor.anchor->kind = 4;
        shop->mode = 12;
        PsynergyMenu_InitializeEntryObjectsFar(party_window, 2, 0, 8, result);
        item_window = UiWindow_CreateFar(16, 11, 14, 9, 2);
        redraw = 1;
        cursor = 0;

        for (;;) {
            if (ask_again != 0) {
                ask_again = 0;
                UiMessage_ShowAndWait(0xc9d);
                redraw = 1;
            }
            if (redraw != 0) {
                redraw = 0;
                cursor = Math_Mod(cursor + shop->party_member_count, shop->party_member_count);
                member = shop->party_member_ids[cursor];
                Shop_PlaceCursor((void *)party_window, cursor * 24 - 12, 0);
                shop->mode = 3;
                Shop_UpdatePartyMemberList(party_window, cursor, shop->selected_item);
                if (Item_GetEquipmentGroupFar(shop->selected_item) == 0)
                    Shop_DrawPartyMemberItems(item_window, member, shop->selected_item);
                else
                    Func_080b1260(item_window, member, shop->selected_item);
            }
            if ((INPUT_NEW_KEYS & 1) != 0) {
                slot = Inventory_AddItemFar(member, shop->selected_item);
                if (slot < 0) {
                    Audio_PlayCue(0x71);
                    UiWork_PushValueSlotFar(member, 1);
                    UiWork_PushValueSlotFar(shop->selected_item, 2);
                    if (Inventory_CountFar(member) == 15)
                        UiMessage_ShowAndWait(0xc9e);
                    else
                        UiMessage_ShowAndWait(0xca6);
                    continue;
                }
                Inventory_RemoveFar(member, slot);
                if ((u32)item->price > SHOP_PARTY_STATE.money)
                    goto too_expensive;
                if (Item_IsCompatibleWithOwnerFar(member, shop->selected_item) == 0) {
                    UiWork_PushValueSlotFar(member, 1);
                    UiMessage_ShowAndWait(0xc9f);
                    ask_again = 1;
                    if (UiMessage_ShowChoice(0) != 0)
                        continue;
                }
                Audio_PlayCue(0x70);
                WaitFrames(1);
                quantity = Shop_SelectPurchaseQuantity(member, shop->selected_item);
                ask_again = 1;
                if (quantity == -1)
                    continue;
                Shop_CompletePurchase(member, shop->selected_item, quantity);
                Shop_BuySpecialItem(party_window, item_window);
                result = 0;
                goto close_party;
            }
            if ((INPUT_NEW_KEYS & 2) != 0)
                goto party_cancelled;
            if ((INPUT_REPEAT_KEYS & 0x20) != 0) {
                Audio_PlayCue(0x6f);
                cursor--;
                redraw = 1;
            }
            if ((INPUT_REPEAT_KEYS & 0x10) != 0) {
                Audio_PlayCue(0x6f);
                cursor++;
                redraw = 1;
            }
            WaitFrames(1);
        }

    close_party:
        Menu_ReleaseEntryObjectsFar();
        UiWork_FinalizeFar(item_window, 2);
        UiWork_FinalizeFar(party_window, 2);
        WaitFrames(1);
        if (result == 0 && shop->party_action == 2) {
            for (i = result; i < quantity; i++)
                Func_08077240(shop->selected_item, -1);
            if (AbilityMenu_BuildAvailableList() == 0)
                goto leave;
            if (selected > shop->stock_count - 1)
                selected = shop->stock_count - 1;
        }
        UiMessage_ShowAndWait(0xca8);
        goto outer;
    }
party_cancelled:
    Audio_PlayCue(0x71);
    result = -1;
    goto close_party;

too_expensive:
    Audio_PlayCue(0x71);
    UiMessage_ShowAndRestoreState(0xc9c);
    result = -1;
    goto close_party;

stock_cancelled:
    Audio_PlayCue(0x71);
    result = -1;
    goto close_stock;

stock_chosen:
    selected = cursor;
    Audio_PlayCue(0x70);
    result = 0;
    goto close_stock;

leave:
    UiWork_FinalizeFar(price_window, 2);
    UiWork_FinalizeFar(shop->money_window, 2);
    WaitFrames(1);
    return 0;
}
