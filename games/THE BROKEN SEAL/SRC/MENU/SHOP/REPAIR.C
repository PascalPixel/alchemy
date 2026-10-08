#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "SHOP.H"
#include "BTLSYS.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "TBS_EDITION.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "text/MSG_IDS.H"
struct ItemDefinition *Item_GetDirectFar(s32 item);

#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Inventory_FindEquippedFar ((s32 (*)(s32,u8))Inventory_FindEquippedFar)
#define Party_AdjustSixDigitCounterBFar ((void (*)(s32))Party_AdjustSixDigitCounterBFar)
#define Item_AdjustCounterFar ((void (*)(s32,s32))Item_AdjustCounterFar)

extern struct ShopRuntime *gMenuWork;
void UiWork_FinalizeFar(s32, s32);
void AudioCommand_PlayFar(s32);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiWork_FinalizePendingCoreFar(void);
void AudioCommand_PlayFar(s32 cue);
#define SPECIAL_ITEM 228

s32 Shop_SelUnit(void)
{
    Shop_PickUnit();
    return 0;
}

s32 Shop_SelRepair(s32 unit_id)
{
    struct ShopRuntime *shop;
    s32 item_count;
    s32 price_window;
    s32 list_window;
    struct BtlUnit *unit;
    s32 selection;
    s32 redraw;
    s32 result;
    s32 item_id;
    s32 price;
    void *window;
    s32 x;
    s32 y;

    shop = gMenuWork;
    unit = UnitGetFar(unit_id);
    item_count = 1;
    list_window = UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, 2);
    selection = 0;

    for (;;) {
        price_window = UiWindow_CreateFar(0, 5, 30, 3, 2);
        shop->cursor.anchor->active = 18;
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
                x = selection % 5 * 16;
                y = selection / 5 * 16 + 8;
                Shop_PlaceCursor(window, x, y);
                shop->mode = 3;
                price = Shop_RepairPrice(unit->inventory[selection]);
                Shop_DrawItemPrice(list_window, item_id, price, 2);
                Shop_DrawMsg(
                    price_window, item_id + (s32)MsgItemPlainName);
            }
            if ((gKeyState & KEY_A) != 0) {
                AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
                result = 0;
                goto done;
            }
            if ((gKeyState & KEY_B) != 0) {
                AudioCommand_PlayFar(SOUND_MENU_CANCEL);
                result = -1;
                goto done;
            }
            if ((gKeysRepeat & KEY_LEFT) != 0) {
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
                selection -= 1;
                selection = (selection + item_count) % item_count;
                redraw = 1;
            }
            if ((gKeysRepeat & KEY_RIGHT) != 0) {
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
                selection += 1;
                selection = (selection + item_count) % item_count;
                redraw = 1;
            }
            if ((gKeysRepeat & KEY_UP) != 0) {
                selection -= 5;
                if (selection < 0)
                    selection += 15;
                while (selection >= item_count)
                    selection -= 5;
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
            }
            if ((gKeysRepeat & KEY_DOWN) != 0) {
                selection += 5;
                if (selection >= item_count)
                    selection -= 15;
                while (selection < 0)
                    selection += 5;
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
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
        UiMessage_ShowAndWait((s32)MsgRepairAnythingElse);
        if (Inventory_CountFar(unit_id) == 0)
            break;
    }

    UiWork_FinalizeFar(list_window, 2);
    return result;
}

void Shop_RepairItem(s32 unit_id, s32 slot)
{
    struct ShopRuntime *shop = gMenuWork;
    struct BtlUnit *unit = UnitGetFar(unit_id);
    s32 item_id = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_GetDirectFar(item_id);
    s32 equipped = Inventory_FindEquippedFar(unit_id, item->type);
    u32 price = Shop_RepairPrice(unit->inventory[slot]);
    s32 message;
    u8 kind;
    u32 saved;

    kind = item->use_type;
    if (kind != 2) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)MsgCannotRepairKind);
        return;
    }
    if (!(unit->inventory[slot] & 0x400)) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)MsgNotDamaged);
        return;
    }
    if ((unit->inventory[slot] & 0x200) && (kind & item->flags)) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)MsgCursedWontComeOff);
        return;
    }
    if (price > gGameState.coins) {
        UiMessage_ShowAndRestoreState((s32)MsgNotEnoughMoney);
        return;
    }
    UiWork_PushValueSlotFar(item_id, 2);
    UiWork_PushValueSlotFar(price, 5);
    message = (s32)MsgShopRepairConfirm;
    UiMessage_ShowAndRestoreState(message);
    if (UiMessage_ShowChoice(0) != 0) {
        UiMessage_ShowAndRestoreState(message + 1);
        return;
    }
    saved = unit->inventory[slot];
    unit->inventory[slot] = 0;
    Shop_DrawUnitGrid(shop->item_window, unit_id);
    UiWork_PushValueSlotFar(item_id, 2);
    UiMessage_ShowAndRestoreState(message + 2);
    UiWork_FinalizePendingCoreFar();
    WaitFrames(10);
    AudioCommand_PlayFar(100);
    WaitFrames(110);
    AudioCommand_PlayFar(100);
    WaitFrames(110);
    AudioCommand_PlayFar(100);
    WaitFrames(110);
    AudioCommand_PlayFar(112);
    WaitFrames(20);
    unit->inventory[slot] = saved;
    Inventory_RepairFar(unit_id, slot);
    Party_AdjustCoinsFar(-price);
    Shop_DrawMoney();
    Shop_DrawUnitGrid(shop->item_window, unit_id);
    UiWork_PushValueSlotFar(item_id, 2);
    UiMessage_ShowAndRestoreState(message + 3);
    if (Shop_ConfirmEquip(unit_id, slot))
        Shop_SellOld(unit_id, equipped);
}

void Shop_BuySpecialItem(void *window, s32 item_window)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 price;
    u32 saved;
    s32 redraw;
    s32 unit_id;
    s32 selected_index;
    s32 message;
    s32 slot;

    redraw = 1;
    unit_id = 0;
    saved = shop->selected_item;
    price = Shop_SpecialItemPrices[gGameState.shop_gifts];
    selected_index = 0;

    if (price > gGameState.shop_credit)
        return;
    shop->selected_item = SPECIAL_ITEM;
    UiWork_PushValueSlotFar(SPECIAL_ITEM, 2);
    message = (s32)MsgTokenGift;
    UiMessage_ShowAndRestoreState(message);
    UiWork_PushValueSlotFar(shop->selected_item, 2);
    UiMessage_ShowAndRestoreState(message + 1);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selected_index = (selected_index + shop->party_member_count) % shop->party_member_count;
            unit_id = shop->party_member_ids[selected_index];
            Shop_PlaceCursor(window, selected_index * 24 - 12, 0);
            shop->mode = 3;
            Shop_DrawParty((s32)window, selected_index, shop->selected_item);
            Shop_DrawUnitItem(item_window, unit_id, shop->selected_item);
        }
        if ((gKeyState & KEY_A) != 0) {
            slot = Inventory_AddItemFar(unit_id, shop->selected_item);
            if (slot < 0) {
                AudioCommand_PlayFar(0x71);
                UiWork_PushValueSlotFar(unit_id, 1);
                UiWork_PushValueSlotFar(shop->selected_item, 2);
                if (Inventory_CountFar(unit_id) == 15)
                    UiMessage_ShowAndWait((s32)MsgPackFull);
                else
                    UiMessage_ShowAndWait((s32)MsgCannotCarryItem);
                continue;
            }
            Inventory_RemoveFar(unit_id, slot);
            AudioCommand_PlayFar(0x65);
            UiMessage_ShowAndRestoreState((s32)MsgHereYouGo);
            Inventory_AddItemFar(unit_id, shop->selected_item);
            Party_AdjustSixDigitCounterBFar(-price);
            Party_AdjustCounterCappedAt28Far(1);
            goto done;
        }
        if ((gKeyState & KEY_B) != 0) {
            UiMessage_ShowAndRestoreState((s32)MsgDontWantIt);
            AudioCommand_PlayFar(0x71);
            goto done;
        }
        if (((gKeysRepeat) & KEY_LEFT) != 0) {
            AudioCommand_PlayFar(0x6f);
            selected_index--;
            redraw = 1;
        }
        if (((gKeysRepeat) & KEY_RIGHT) != 0) {
            AudioCommand_PlayFar(0x6f);
            selected_index++;
            redraw = 1;
        }
        WaitFrames(1);
    }
done:
    shop->selected_item = saved;
}

s32 EventTable_GetRowLimit(void)
{
    return 35;
}

void EventTable_ApplyRowAbilities(s32 row_no)
{
    s16 *row;
    s16 *entry;
    s32 value;
    s32 count;
    s32 flag;

    flag = row_no + 0x400;
    if (GameFlag_TestFar(flag) == 0) {
        GameFlag_SetBitFar(flag);
        count = 0;
        value = Shop_StockTable[row_no][24];
        if (value != 0) {
            row = Shop_StockTable[row_no];
            entry = row + 24;
            do {
                Item_AdjustCounterFar(value, 1);
                count++;
                if (count > 7)
                    break;
                entry++;
                value = *entry;
            } while (value != 0);
        }
    }
}

s32 EventTable_CopyRowHeader(s32 row_no, s16 *output)
{
    s16 *src;
    s16 *dst;
    s32 count;

    count = 0;
    if (Shop_StockTable[row_no][0] != 0) {
        dst = output;
        src = Shop_StockTable[row_no];
        do {
            /* FAKEMATCH: only the copy read is volatile, so its lifetime stays separate from the signed sentinel read. */
            *dst = *(volatile s16 *)src;
            count++;
            src++;
            dst++;
            if (count > 23)
                break;
        } while (*src != 0);
    }
    output[count] = 0;
    return count;
}

s32 EventTable_GetRowType(s32 index)
{
    return Shop_StockTable[index][32];
}
