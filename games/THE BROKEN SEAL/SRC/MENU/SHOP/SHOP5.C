#include "SHOP.H"
#include "BATTLE_RUNTIME.H"
#include "SOUND_IDS.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "UI.H"
#include "TBS_EDITION.H"
#include "PARTY_STATE.H"
#include "TYPES.H"
#include "IO_REG.H"

extern struct ShopRuntime *gMenuWork;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
void UiWork_FinalizeFar(s32, s32);
s32 Inventory_CountFar(s32);
void Audio_PlayCue(s32);
void UiMessage_ShowAndWait(s32);
void Shop_RepairItem(s32, s32);
extern u8 MsgItemPlainName;
extern u8 MsgRepairAnythingElse;
extern u8 MsgShopRepairConfirm[];
extern u8 MsgCannotRepairKind[];
extern u8 MsgNotDamaged[];
extern u8 MsgCursedWontComeOff[];
extern u8 MsgNotEnoughMoney[];
s32 Inventory_FindEquippedFar(s32 unit_id, u8 kind);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void UiMessage_ShowAndRestoreState(s32 message);
s32 UiMessage_ShowChoice(s32 value);
void UiWork_FinalizePendingCoreFar(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void Func_08077068(s32 unit_id, s32 slot);
void Party_AdjustSixDigitCounterAFar(s32 amount);
#define SPECIAL_ITEM 228

extern const s16 Shop_SpecialItemPrices[];
extern u8 MsgTokenGift[];
extern u8 MsgPackFull[];
extern u8 MsgCannotCarryItem[];
extern u8 MsgHereYouGo[];
extern u8 MsgDontWantIt[];
void UiMessage_ShowAndWait(s32 message);
s32 Inventory_AddItemFar(s32 unit_id, s32 item_id);
void Inventory_RemoveFar(s32 unit_id, s32 slot);
s32 Inventory_CountFar(s32 unit_id);
void Party_AdjustSixDigitCounterBFar(s32 amount);
void Func_080772a0(s32 value);

extern s16 EventTable_AbilityLoadouts[][33];
s32 GameFlag_TestFar(s32);
s32 GameFlag_SetBitFar(s32);
void Ability_GetMaximum(s32, s32);

s32 Shop_SelUnit(void)
{
    Shop_PickUnit();
    return 0;
}

/*
 * Repair flow reached from Shop_PickUnit when the shop's party
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

    shop = gMenuWork;
    unit = Owner_GetStateFar(unit_id);
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
                    price_window, item_id + (s32)&MsgItemPlainName);
            }
            if ((gKeyState & KEY_A) != 0) {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
                result = 0;
                goto done;
            }
            if ((gKeyState & KEY_B) != 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                result = -1;
                goto done;
            }
            if ((gKeysRepeat & KEY_LEFT) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                selection -= 1;
                selection = (selection + item_count) % item_count;
                redraw = 1;
            }
            if ((gKeysRepeat & KEY_RIGHT) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
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
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
            }
            if ((gKeysRepeat & KEY_DOWN) != 0) {
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
        UiMessage_ShowAndWait((s32)&MsgRepairAnythingElse);
        if (Inventory_CountFar(unit_id) == 0)
            break;
    }

    UiWork_FinalizeFar(list_window, 2);
    return result;
}

/* Repairs one item for a party member at the shop: refuses items that
   cannot be repaired, are not broken or cost more than the party has, and
   otherwise takes the money, plays the smithing sounds and offers to equip
   the mended item. */
void Shop_RepairItem(s32 unit_id, s32 slot)
{
    struct ShopRuntime *shop = gMenuWork;
    struct BattleUnit *unit = Owner_GetStateFar(unit_id);
    s32 item_id = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_Get(item_id);
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
    Audio_PlayCue(100);
    WaitFrames(110);
    Audio_PlayCue(100);
    WaitFrames(110);
    Audio_PlayCue(100);
    WaitFrames(110);
    Audio_PlayCue(112);
    WaitFrames(20);
    unit->inventory[slot] = saved;
    Func_08077068(unit_id, slot);
    Party_AdjustSixDigitCounterAFar(-price);
    Shop_DrawMoney();
    Shop_DrawUnitGrid(shop->item_window, unit_id);
    UiWork_PushValueSlotFar(item_id, 2);
    UiMessage_ShowAndRestoreState(message + 3);
    if (Shop_ConfirmEquip(unit_id, slot))
        Shop_SellOld(unit_id, equipped);
}

/* Offers the shop's special item, whose price rises with each purchase:
   when the party can afford it, the player picks the member who carries
   it, and a full bag sends them back to choose again. */
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
                Audio_PlayCue(0x71);
                UiWork_PushValueSlotFar(unit_id, 1);
                UiWork_PushValueSlotFar(shop->selected_item, 2);
                if (Inventory_CountFar(unit_id) == 15)
                    UiMessage_ShowAndWait((s32)MsgPackFull);
                else
                    UiMessage_ShowAndWait((s32)MsgCannotCarryItem);
                continue;
            }
            Inventory_RemoveFar(unit_id, slot);
            Audio_PlayCue(0x65);
            UiMessage_ShowAndRestoreState((s32)MsgHereYouGo);
            Inventory_AddItemFar(unit_id, shop->selected_item);
            Party_AdjustSixDigitCounterBFar(-price);
            Func_080772a0(1);
            goto done;
        }
        if ((gKeyState & KEY_B) != 0) {
            UiMessage_ShowAndRestoreState((s32)MsgDontWantIt);
            Audio_PlayCue(0x71);
            goto done;
        }
        if (((gKeysRepeat) & KEY_LEFT) != 0) {
            Audio_PlayCue(0x6f);
            selected_index--;
            redraw = 1;
        }
        if (((gKeysRepeat) & KEY_RIGHT) != 0) {
            Audio_PlayCue(0x6f);
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
        value = EventTable_AbilityLoadouts[row_no][24];
        if (value != 0) {
            row = EventTable_AbilityLoadouts[row_no];
            entry = row + 24;
            do {
                Ability_GetMaximum(value, 1);
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
    if (EventTable_AbilityLoadouts[row_no][0] != 0) {
        dst = output;
        src = EventTable_AbilityLoadouts[row_no];
        do {
            /* FAKEMATCH: only the copy read is volatile, so its lifetime
               stays separate from the signed sentinel read. */
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
    return EventTable_AbilityLoadouts[index][32];
}
