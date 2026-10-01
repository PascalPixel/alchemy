#include "SHOP.H"
#include "BATTLE_RUNTIME.H"
#include "TBS_EDITION.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"

extern struct ShopRuntime *gMenuWork;
extern u8 Data_03001f2c[];
extern u8 Data_03001c94[];
extern u8 gKeysRepeat[];
s32 Inventory_AddItemFar(s32, s32);
s32 Inventory_FindEquippedFar(s32, u8);
s32 Party_AdjustSixDigitCounterAFar(s32);
s32 Party_AdjustSixDigitCounterBFar(s16);
void UiMessage_ShowAndRestoreState(s32 message);
void Audio_PlayCue(s32);
extern char MsgHereYouGo;
s32 Item_CanOwnerEquip(s32 unit_id, s32 item_id);
s32 Inventory_FindEquippedFar(s32 unit_id, u8 kind);
void Inventory_EquipFar(s32 unit_id, s32 slot);
void UiWork_PushValueSlotFar(u32 unit_id, u32 mode);
void UiMessage_ShowAndWait(s32 message);
s32 UiMessage_ShowChoice(s32 value);
void Shop_DrawUnitGrid(s32 value, s32 unit_id);
void Audio_PlayCue(s32 cue);
void UiWork_FinalizePendingCoreFar(void);
s32 UiText_OpenMessageWindowFar(s32 a, s32 b, s32 c, s32 d);
extern char MsgEquipNowPrompt;
extern char MsgLookBolder;
extern u8 MsgBecameCursed[];
void Shop_SellItem(s32, s32, s32);
void UiWork_FinalizeFar(s32, s32);
s32 Inventory_CountFar(s32);
void PsynergyMenu_InitializeEntryObjectsFar(s32, s32, s32, s32, s32);
void Menu_ReleaseEntryObjectsFar(void);
s32 Shop_SelSell(s32);
void Shop_SelRepair(s32);
void UiMessage_ShowAndWait(s32);
extern u8 MsgItemPlainName;
extern u8 MsgSellAnythingElse;
extern u8 MsgNoItems;
void UiWindow_Clear(s32 window);
s32 Inventory_CountFar(s32 unit_id);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
u8 *UiIcon_Draw(u16 no, s32 kind, s32 window, s32 x, s32 y);
s32 Shop_GetSelectionState(s32, s32);
s32 Shop_SelectQuantity(s32, s32, s32);
void UiIcon_PrepareObjectFar(void *);
extern char MsgHowManyToSell;

extern u8 MsgCannotSellEquipped[], MsgWeDoNotBuy[], MsgSellOffer[], MsgRareSellOffer[];
extern u8 MsgSellTotal[], MsgDamagedSellOffer[], MsgOldItemSellOffer[], MsgDeal[];
extern u8 MsgChangedMind[], MsgSold[], MsgKeepIt[];
struct ItemDefinition *Item_Get(s32 item);
void Func_080772b0(s32 unit_id, s32 slot);

void Shop_BuyDone(s32 unit_id, s32 item_id, s32 quantity)
{
    s32 replaced_slot;
    s32 remaining;
    s32 added_slot;
    struct ItemDefinition *item;

    remaining = quantity;
    item = Item_Get(item_id);
    added_slot = 0;
    replaced_slot = Inventory_FindEquippedFar(unit_id, item->type);
    Audio_PlayCue(SOUND_SHOP_PURCHASE);
    if (added_slot < remaining) {
        do {
            added_slot = Inventory_AddItemFar(unit_id, item_id);
            Party_AdjustSixDigitCounterAFar(0 - item->price);
            remaining -= 1;
            Party_AdjustSixDigitCounterBFar(item->price);
            Shop_DrawMoney();
        } while (remaining != 0);
    }
    UiMessage_ShowAndRestoreState((s32)&MsgHereYouGo);
    if (Shop_ConfirmEquip(unit_id, added_slot) != 0) {
        Shop_SellOld(unit_id, replaced_slot);
    }
}

s32 Shop_ConfirmEquip(s32 unit_id, s32 slot)
{
    struct ShopRuntime *menu = gMenuWork;
    struct BattleUnit *unit = (struct BattleUnit *)Owner_GetStateFar(unit_id);
    s32 item_id = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *info = Item_Get(item_id);
    s32 replaced;
    s32 menu_value;

    if (unit->inventory[slot] & 0x200)
        return 0;

    if (Item_CanOwnerEquip(unit_id, item_id) == 0)
        return 0;

    replaced = Inventory_FindEquippedFar(unit_id, info->type);
    if (replaced != -1) {
        struct ItemDefinition *old_info = Item_Get(unit->inventory[replaced]);

        if (old_info->flags & 2)
            return 0;
    }

    UiWork_PushValueSlotFar(unit_id, 1);
    UiMessage_ShowAndWait((s32)&MsgEquipNowPrompt);
    if (UiMessage_ShowChoice(0) != 0)
        return 0;

    Inventory_EquipFar(unit_id, slot);
    menu_value = menu->item_window;
    if (menu_value != 0)
        Shop_DrawUnitGrid(menu_value, unit_id);

    if (info->flags & 1) {
        Audio_PlayCue(103);
        UiWork_FinalizePendingCoreFar();
        UiText_OpenMessageWindowFar((s32)MsgBecameCursed, 8, 4, 2);
        while (UiWork_IsCompleteFar() == 0) {
            WaitFrames(1);
        }
    }

    UiMessage_ShowAndRestoreState((s32)&MsgLookBolder);
    return 1;
}

s32 Shop_SellOld(s32 unit_id, s32 slot)
{
    struct BattleUnit *unit;
    s32 item_offset;
    s32 item_id;

    unit = Owner_GetStateFar(unit_id);
    if (slot == -1)
    {
        return 0;
    }
    item_offset = slot * 2 + 216;
    item_id = 0x1ff & *(u16 *)((u8 *)unit + item_offset);
    if (Item_Get(item_id)->type == 6)
    {
        return 0;
    }
    if (Item_Get(item_id)->flags & 8)
    {
        return 0;
    }
    Shop_SellItem(unit_id, slot, -1);
    return 1;
}

s32 Shop_SalePrice(s32 item_id)
{
    s32 price;

    price = Item_Get(item_id)->price;

    if (Item_Get(item_id)->flags & 8) {
        price = 0;
    } else if (item_id & 0x400) {
        price = price / 2;
    } else {
        price = price * 3 / 4;
    }
    return price;
}

#if defined(TBS_EDITION_JA)
#define BASE_W 11
#else
#define BASE_W 12
#endif

/*
 * Keep an actor-selection menu active while dispatching the chosen actor into
 * one of two action screens.  The menu itself closes only when cancelled.
 */
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
    shop->cursor.anchor->kind = 4;
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

        if ((*(volatile u32 *)((u32)&Data_03001c94) & 1) != 0) {
            WaitFrames(1);
            if (Inventory_CountFar(unit_id) == 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
            } else {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
                if (shop->party_action == 1)
                    Shop_SelSell(unit_id);
                else
                    Shop_SelRepair(unit_id);
                shop->cursor.anchor->kind = 4;
                shop->mode = 12;
                redraw = 1;
            }
            continue;
        }

        if ((*(volatile u32 *)((u32)&Data_03001c94) & 2) != 0) {
            Audio_PlayCue(SOUND_MENU_CANCEL);
            Menu_ReleaseEntryObjectsFar();
            UiWork_FinalizeFar(list_window, 2);
            UiWork_FinalizeFar(shop->item_window, 2);
            UiWork_FinalizeFar(shop->money_window, 2);
            WaitFrames(1);
            return 0;
        }

        if ((*(volatile u32 *)((u32)&gKeysRepeat) & 0x20) != 0) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            selection--;
            redraw = 1;
        }
        if ((*(volatile u32 *)((u32)&gKeysRepeat) & 0x10) != 0) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            selection++;
            redraw = 1;
        }
        WaitFrames(1);
    }
}

/*
 * Sell flow reached from Shop_PickUnit when the shop's party action
 * is "sell": browse the chosen member's inventory, priced one slot at a
 * time, and hand a confirmed slot off to Shop_SelSellNum before
 * writing the sale back through Shop_SellItem.
 */
s32 Shop_SelSell(s32 unit_id)
{
    s32 price_window;
    struct ShopRuntime *shop;
    s32 list_window;
    struct BattleUnit *unit;
    s32 selection;
    s32 item_count;
    s32 redraw;
    s32 result;
    s32 item_id;
    s32 quantity;
    void *window;
    s32 x;

    shop = gMenuWork;
    unit = Owner_GetStateFar(unit_id);
    item_count = 1;
    list_window = UiWindow_CreateFar(SHOP_LIST_X, 8, SHOP_LIST_WIDTH, 4, 2);
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
                x = selection % 5 * 16;
                Shop_PlaceCursor(window, x, selection / 5 * 16 + 8);
                shop->mode = 3;
                Shop_DrawItemPrice(
                    list_window,
                    item_id,
                    Shop_SalePrice(unit->inventory[selection]),
                    1);
                Shop_DrawMsg(price_window, item_id + (s32)&MsgItemPlainName);
            }

            if ((*(volatile u32 *)((u32)&Data_03001c94) & 1) != 0) {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
                result = 0;
                goto done;
            }
            if ((*(volatile u32 *)((u32)&Data_03001c94) & 2) != 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                result = -1;
                goto done;
            }
            if ((*(volatile u32 *)((u32)&gKeysRepeat) & 0x20) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                selection -= 1;
                selection = (selection + item_count) % item_count;
                redraw = 1;
            }
            if ((*(volatile u32 *)((u32)&gKeysRepeat) & 0x10) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                selection += 1;
                selection = (selection + item_count) % item_count;
                redraw = 1;
            }
            if ((*(volatile u32 *)((u32)&gKeysRepeat) & 0x40) != 0) {
                selection -= 5;
                if (selection < 0)
                    selection += 15;
                while (selection >= item_count)
                    selection -= 5;
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
            }
            if ((*(volatile u32 *)((u32)&gKeysRepeat) & 0x80) != 0) {
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

        quantity = Shop_SelSellNum(unit_id, selection);
        if (quantity != -1)
            Shop_SellItem(unit_id, selection, quantity);
        UiMessage_ShowAndWait((s32)&MsgSellAnythingElse);
        if (Inventory_CountFar(unit_id) == 0)
            break;
    }

    UiWork_FinalizeFar(list_window, 2);
    return result;
}

void Shop_DrawUnitGrid(s32 window, s32 unit_id)
{
    u8 *unit;
    s32 x;
    s32 y;
    s32 slot;
    s32 item_offset;
    u8 *icon;

    unit = (u8 *)Owner_GetStateFar(unit_id);
    x = 8;
    y = 0;
    if (window != 0) {
        UiWindow_Clear(window);
        if (Inventory_CountFar(unit_id) == 0) {
#if defined(TBS_EDITION_IT)
            UiText_DrawMessageAt((s32)&MsgNoItems, window, 20, 12);
#else
            UiText_DrawMessageAt((s32)&MsgNoItems, window, 8, 20);
#endif
        } else {
            slot = 0;
            item_offset = 216;
            do {
                if (*(u16 *)((u32)item_offset + (u32)unit) != 0) {
                    icon = UiIcon_Draw(
                        *(u16 *)((u32)item_offset + (u32)unit),
                        27, window, x, y);
                    icon[15] = 252;
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
                item_offset += 2;
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
    s32 entry_offset;
    s32 selection;
    struct ItemDefinition *item;
    struct ShopRuntime *shop;
    struct BattleUnit *unit;

    shop = gMenuWork;
    unit = Owner_GetStateFar(unit_id);
    entry_offset = (slot * 2) + 0xd8;
    item = Item_Get(*(u16 *)((u8 *)unit + entry_offset));
    result = 1;
    effect = Shop_SalePrice(*(u16 *)((u8 *)unit + entry_offset));
    state = Shop_GetSelectionState(unit_id, slot);
    selection = state;
    if ((item->flags & 0x10) && state > 1) {
        UiMessage_ShowAndWait((s32)&MsgHowManyToSell);
        saved_x = shop->cursor.target_x;
        saved_y = shop->cursor.target_y;
        shop->cursor.anchor->kind = 4;
        shop->mode = 0xc;
        Shop_PlaceCursor(NULL, EFFECT_X, 0x30);
        result = Shop_SelectQuantity(0, selection, effect);
        WaitFrames(1);
        UiIcon_PrepareObjectFar(shop->cursor.anchor);
        Shop_PlaceCursor(NULL, saved_x, saved_y);
    }
    return result;
}

/* Sells count of one inventory slot (count -1 sells the whole stack as
   one): refuses worthless and cursed-and-equipped items, picks the offer
   text for broken, several, rare or ordinary items, and on agreement
   removes the items and pays out. */
void Shop_SellItem(s32 unit_id, s32 slot, s32 count)
{
    struct ShopRuntime *shop = gMenuWork;
    struct BattleUnit *unit = Owner_GetStateFar(unit_id);
    s32 item_id = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_Get(item_id);
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
    Audio_PlayCue(102);
    for (i = 0; i < count; i++)
        Func_080772b0(unit_id, slot);
    Party_AdjustSixDigitCounterAFar(total);
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
    s32 result = Item_Get(item_id)->price / 4;

    if ((item_id & 0x400) == 0) {
        result = 0;
    }
    return result;
}
