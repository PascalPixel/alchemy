#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "IO_REG.H"
#include "EDITION.H"
#include "SHOP.H"
#include "BTLSYS.H"
#include "TBS_EDITION.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "ITEM.H"
#include "text/MSG_IDS.H"
void RenderOutput_PrepareForRedrawFar(s32 window);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
struct ItemDefinition *Item_GetDirectFar(s32 item);
struct RenderOutput *RenderOutput_CreateLoadedFar(s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y);

#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Inventory_FindEquippedFar ((s32 (*)(s32,u8))Inventory_FindEquippedFar)
#define Party_AdjustSixDigitCounterBFar ((s32 (*)(s16))Party_AdjustSixDigitCounterBFar)
#define Inventory_FindEquippedFar ((s32 (*)(s32,u8))Inventory_FindEquippedFar)
#define Inventory_EquipFar ((void (*)(s32,s32))Inventory_EquipFar)
#define RenderOutput_CreateLoadedFar ((struct RenderOutput * (*)(u16,s32,s32,s32,s32))RenderOutput_CreateLoadedFar)
#define UiIcon_PrepareObjectFar ((void (*)(void*))UiIcon_PrepareObjectFar)
#define Inventory_DiscardFar ((void (*)(s32,s32))Inventory_DiscardFar)

extern struct ShopRuntime *gMenuWork;
void AudioCommand_PlayFar(s32);
void UiWork_PushValueSlotFar(u32 unit_id, u32 mode);
void AudioCommand_PlayFar(s32 cue);
void UiWork_FinalizePendingCoreFar(void);
void UiWork_FinalizeFar(s32, s32);
void PsynergyMenu_InitializeEntryObjectsFar();

void Shop_BuyDone(s32 unit_id, s32 item_id, s32 quantity)
{
    s32 replaced_slot;
    s32 remaining;
    s32 added_slot;
    struct ItemDefinition *item;

    remaining = quantity;
    item = Item_GetDirectFar(item_id);
    added_slot = 0;
    replaced_slot = Inventory_FindEquippedFar(unit_id, item->type);
    AudioCommand_PlayFar(SOUND_SHOP_PURCHASE);
    if (added_slot < remaining) {
        do {
            added_slot = Inventory_AddItemFar(unit_id, item_id);
            Party_AdjustCoinsFar(0 - item->price);
            remaining -= 1;
            Party_AdjustSixDigitCounterBFar(item->price);
            Shop_DrawMoney();
        } while (remaining != 0);
    }
    UiMessage_ShowAndRestoreState((s32)MsgHereYouGo);
    if (Shop_ConfirmEquip(unit_id, added_slot) != 0) {
        Shop_SellOld(unit_id, replaced_slot);
    }
}

s32 Shop_ConfirmEquip(s32 unit_id, s32 slot)
{
    struct ShopRuntime *menu = gMenuWork;
    struct BtlUnit *unit = (struct BtlUnit *)UnitGetFar(unit_id);
    s32 item_id = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *info = Item_GetDirectFar(item_id);
    s32 replaced;
    s32 menu_value;

    if (unit->inventory[slot] & 0x200)
        return 0;

    if (Item_CanOwnerEquip(unit_id, item_id) == 0)
        return 0;

    replaced = Inventory_FindEquippedFar(unit_id, info->type);
    if (replaced != -1) {
        struct ItemDefinition *old_info = Item_GetDirectFar(unit->inventory[replaced]);

        if (old_info->flags & 2)
            return 0;
    }

    UiWork_PushValueSlotFar(unit_id, 1);
    UiMessage_ShowAndWait((s32)MsgEquipNowPrompt);
    if (UiMessage_ShowChoice(0) != 0)
        return 0;

    Inventory_EquipFar(unit_id, slot);
    menu_value = menu->item_window;
    if (menu_value != 0)
        Shop_DrawUnitGrid(menu_value, unit_id);

    if (info->flags & 1) {
        AudioCommand_PlayFar(103);
        UiWork_FinalizePendingCoreFar();
        UiText_OpenMessageWindowFar((s32)MsgBecameCursed, 8, 4, 2);
        while (UiWork_IsCompleteFar() == 0) {
            WaitFrames(1);
        }
    }

    UiMessage_ShowAndRestoreState((s32)MsgLookBolder);
    return 1;
}

s32 Shop_SellOld(s32 unit_id, s32 slot)
{
    struct BtlUnit *unit;
    s32 item_id;

    unit = UnitGetFar(unit_id);
    if (slot == -1)
    {
        return 0;
    }
    item_id = unit->inventory[slot] & 0x1ff;
    if (Item_GetDirectFar(item_id)->type == 6)
    {
        return 0;
    }
    if (Item_GetDirectFar(item_id)->flags & 8)
    {
        return 0;
    }
    Shop_SellItem(unit_id, slot, -1);
    return 1;
}

s32 Shop_SalePrice(s32 item_id)
{
    s32 price;

    price = Item_GetDirectFar(item_id)->price;

    if (Item_GetDirectFar(item_id)->flags & 8) {
        price = 0;
    } else if (item_id & 0x400) {
        price = price / 2;
    } else {
        price = price * 3 / 4;
    }
    return price;
}

#if EDITION_INTERNATIONAL
#define BASE_W 12
#else
#define BASE_W 11
#endif

s32 Shop_PickUnit(void)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 list_window;
    s32 selection = 0;
    s32 redraw = 1;
    s32 unit_id = 0;

    shop->money_window = UiWindow_CreateFar(0, 9, BASE_W, 4, 2);
    Shop_DrawMoney();
    shop->item_window = UiWindow_CreateFar(16, 12, 14, 8, 2);
    list_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
    shop->cursor.anchor->active = 4;
    shop->mode = 12;
    PsynergyMenu_InitializeEntryObjectsFar(list_window, 2, 0, 8, 0);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selection = (selection + shop->party_member_count) % shop->party_member_count;
            unit_id = shop->party_member_ids[selection];
            Shop_PlaceCursor(
                (void *)list_window,
                selection * 24 - 12,
                0);
            shop->mode = 3;
            Shop_DrawParty(list_window, selection, 0);
            Shop_DrawUnitGrid(shop->item_window, unit_id);
        }

        if ((gKeyState & KEY_A) != 0) {
            WaitFrames(1);
            if (Inventory_CountFar(unit_id) == 0) {
                AudioCommand_PlayFar(SOUND_MENU_CANCEL);
            } else {
                AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
                if (shop->party_action == 1)
                    Shop_SelSell(unit_id);
                else
                    Shop_SelRepair(unit_id);
                shop->cursor.anchor->active = 4;
                shop->mode = 12;
                redraw = 1;
            }
            continue;
        }

        if ((gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(SOUND_MENU_CANCEL);
            Menu_ReleaseEntryObjectsFar();
            UiWork_FinalizeFar(list_window, 2);
            UiWork_FinalizeFar(shop->item_window, 2);
            UiWork_FinalizeFar(shop->money_window, 2);
            WaitFrames(1);
            return 0;
        }

        if ((gKeysRepeat & KEY_LEFT) != 0) {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            selection--;
            redraw = 1;
        }
        if ((gKeysRepeat & KEY_RIGHT) != 0) {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            selection++;
            redraw = 1;
        }
        WaitFrames(1);
    }
}

s32 Shop_SelSell(s32 unit_id)
{
    s32 price_window;
    struct ShopRuntime *shop;
    s32 list_window;
    struct BtlUnit *unit;
    s32 selection;
    s32 item_count;
    s32 redraw;
    s32 result;
    s32 item_id;
    s32 quantity;
    void *window;
    s32 x;

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
                Shop_PlaceCursor(window, x, selection / 5 * 16 + 8);
                shop->mode = 3;
                Shop_DrawItemPrice(
                    list_window,
                    item_id,
                    Shop_SalePrice(unit->inventory[selection]),
                    1);
                Shop_DrawMsg(price_window, item_id + (s32)MsgItemPlainName);
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

        quantity = Shop_SelSellNum(unit_id, selection);
        if (quantity != -1)
            Shop_SellItem(unit_id, selection, quantity);
        UiMessage_ShowAndWait((s32)MsgSellAnythingElse);
        if (Inventory_CountFar(unit_id) == 0)
            break;
    }

    UiWork_FinalizeFar(list_window, 2);
    return result;
}

void Shop_DrawUnitGrid(s32 window, s32 unit_id)
{
    struct BtlUnit *unit;
    s32 x;
    s32 y;
    s32 slot;
    struct RenderOutput *icon;

    unit = UnitGetFar(unit_id);
    x = 8;
    y = 0;
    if (window != 0) {
        RenderOutput_PrepareForRedrawFar(window);
        if (Inventory_CountFar(unit_id) == 0) {
#if defined(TBS_EDITION_IT)
            UiText_DrawResourceFar((s32)MsgNoItems, window, 20, 12);
#else
            UiText_DrawResourceFar((s32)MsgNoItems, window, 8, 20);
#endif
        } else {
            slot = 0;
            do {
                if (unit->inventory[slot] != 0) {
                    icon = RenderOutput_CreateLoadedFar(
                        unit->inventory[slot],
                        27, window, x, y);
                    icon->sentinel = 252;
                }
                x += 16;
                if (slot == 4) {
                    x = 8;
                    y += 16;
                }
                if (slot == 9) {
                    x = 8;
                    y += 16;
                }
                slot++;
            } while (slot <= 14);
        }
    }
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define EFFECT_X 0x78
#else
#define EFFECT_X 0x80
#endif

s32 Shop_SelSellNum(s32 unit_id, s32 slot)
{
    s32 result;
    s16 saved_x;
    s16 saved_y;
    s32 effect;
    s32 state;
    s32 selection;
    struct ItemDefinition *item;
    struct ShopRuntime *shop;
    struct BtlUnit *unit;

    shop = gMenuWork;
    unit = UnitGetFar(unit_id);
    item = Item_GetDirectFar(unit->inventory[slot]);
    result = 1;
    effect = Shop_SalePrice(unit->inventory[slot]);
    state = Shop_GetSelectionState(unit_id, slot);
    selection = state;
    if ((item->flags & 0x10) && state > 1) {
        UiMessage_ShowAndWait((s32)MsgHowManyToSell);
        saved_x = shop->cursor.target_x;
        saved_y = shop->cursor.target_y;
        shop->cursor.anchor->active = 4;
        shop->mode = 0xc;
        Shop_PlaceCursor(NULL, EFFECT_X, 0x30);
        result = Shop_SelectQuantity(0, selection, effect);
        WaitFrames(1);
        UiIcon_PrepareObjectFar(shop->cursor.anchor);
        Shop_PlaceCursor(NULL, saved_x, saved_y);
    }
    return result;
}

void Shop_SellItem(s32 unit_id, s32 slot, s32 count)
{
    struct ShopRuntime *shop = gMenuWork;
    struct BtlUnit *unit = UnitGetFar(unit_id);
    s32 item_id = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_GetDirectFar(item_id);
    u8 rare = item->flags & 4;
    s32 all = 0;
    s32 total;
    s32 message;
    s32 i;

    if (count == -1) {
        all = 1;
        count = 1;
    }
    total = Shop_SalePrice(unit->inventory[slot]) * count;
    if (total == 0) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)MsgWeDoNotBuy);
        return;
    }
    if ((unit->inventory[slot] & 0x200) && (item->flags & 2)) {
        UiWork_PushValueSlotFar(item_id, 2);
        UiMessage_ShowAndRestoreState((s32)MsgCannotSellEquipped);
        return;
    }
    if (all)
        message = (s32)MsgOldItemSellOffer;
    else if (unit->inventory[slot] & 0x400)
        message = (s32)MsgDamagedSellOffer;
    else if (count > 1)
        message = (s32)MsgSellTotal;
    else if (rare)
        message = (s32)MsgRareSellOffer;
    else
        message = (s32)MsgSellOffer;
    UiWork_PushValueSlotFar(item_id, 2);
    UiWork_PushValueSlotFar(total, 5);
    UiMessage_ShowAndRestoreState(message);
    if (UiMessage_ShowChoice(0) != 0) {
        if (rare || all)
            message = (s32)MsgKeepIt;
        else
            message = (s32)MsgChangedMind;
        UiMessage_ShowAndRestoreState(message);
        return;
    }
    AudioCommand_PlayFar(102);
    for (i = 0; i < count; i++)
        Inventory_DiscardFar(unit_id, slot);
    Party_AdjustCoinsFar(total);
    Shop_DrawMoney();
    Shop_DrawUnitGrid(shop->item_window, unit_id);
    if (rare || all)
        message = (s32)MsgSold;
    else
        message = (s32)MsgDeal;
    UiMessage_ShowAndRestoreState(message);
}

s32 Shop_RepairPrice(s32 item_id)
{
    s32 result = Item_GetDirectFar(item_id)->price / 4;

    if ((item_id & 0x400) == 0) {
        result = 0;
    }
    return result;
}
