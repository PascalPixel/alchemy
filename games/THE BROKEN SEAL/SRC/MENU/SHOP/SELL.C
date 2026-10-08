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
#include "ITEM.H"
#include "UI.H"
#include "text/MSG_IDS.H"
#include "PARTY_STATE.H"
#include "TBS_EDITION.H"
#include "IO_REG.H"
#include "MEM.H"
#include "EDITION.H"
#include "DMA.H"
#include "FIXED_MATH.H"
#include "RESOURCE.H"
#include "SOUND_IDS.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "OBJECT_RUNTIME.H"
#include "SCENE.H"
#include "FIXED_POINT_POSITION.H"
#include "CALLBACK_SCHEDULER.H"
#include "INN_RUNTIME.H"
#include "EVENT_RUNTIME.H"
#include "BTLCALC.H"

#define Item_IsCompatibleWithOwnerFar ((s32 (*)(s16,s32))Item_IsCompatibleWithOwnerFar)
#define gMenuWork (*(struct ShopRuntime *(*))&gMenuWork)

void Shop_DrawParty(s32 window, s32 selected, s32 requirement)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 index;

    if (window != 0) {
        for (index = 0; index < shop->party_member_count; index++) {
            if (index == selected)
                AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 30);
            else
                AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 1);
            shop->party_member_scale[index] = 0x10000;
            if (!Item_IsCompatibleWithOwnerFar(shop->party_member_ids[index], requirement))
                shop->party_member_scale[index] = 0xcccc;
        }
    }
}

#undef gMenuWork

void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 Item_CanOwnerEquip(s32 owner, s32 item);
struct ItemDefinition *Item_GetDirectFar(s32 item);
#define gMenuWork (*(struct ShopRuntime *(*))&gMenuWork)

void UiWindow_DrawDividerLineFar(s32 window, s32 x, s32 y, s32 width, s32 style);

void Shop_DrawEquipComparison(s32 window, s32 unit_id, s32 item_id)
{
    struct ShopRuntime *shop = gMenuWork;
    struct BtlUnit *unit = UnitGetFar(unit_id);
    struct ItemDefinition *item = Item_GetDirectFar(item_id);
    s32 replaced = -1;
    s32 slot;
    s32 saved;
    s32 i;
    s32 without[4];
    s32 with[4];

    if (window == 0)
        return;
    RenderOutput_PrepareForRedrawFar(window);
    if (!Item_CanOwnerEquip(unit_id, item_id)) {
        UiText_DrawResourceFar((s32)MsgShopCannotEquip, window, 8, 24);
        return;
    }
    slot = Inventory_FindEquippedFar(unit_id, item->type);
    if (slot == -1) {
        for (i = 0; i <= 14; i++) {
            if (!(unit->inventory[i] & 0x200))
                break;
        }
        if (i == 15) {
            for (i = 0; i <= 14; i++) {
                if (Item_GetDirectFar(unit->inventory[i])->type == 6)
                    break;
            }
            if (i == 15)
                i = 0;
        }
        slot = i;
    } else {
        replaced = unit->inventory[slot] & 0x1ff;
    }
    saved = unit->inventory[slot];
    unit->inventory[slot] = item_id | 0x200;
    UnitCalcFar(unit_id);
    with[0] = unit->atk;
    with[1] = unit->def;
    with[2] = unit->agi;
    with[3] = unit->luck;
    unit->inventory[slot] = saved;
    UnitCalcFar(unit_id);
    without[0] = unit->atk;
    without[1] = unit->def;
    without[2] = unit->agi;
    without[3] = unit->luck;
    for (i = 0; i <= 2; i++) {
        if (without[i] > with[i])
            RenderOutput_CreateFar(shop->stat_down_icon, 0x40000000, (struct RenderInput *)window, 56, i * 16 - 4)->kind = 0;
        else if (without[i] < with[i])
            RenderOutput_CreateFar(shop->stat_up_icon, 0x40000000, (struct RenderInput *)window, 56, i * 16 - 4)->kind = 0;
        UiText_DrawNumberInWindowFar(without[i], 3, window, 32, i * 16);
        if (without[i] != with[i])
            UiText_DrawNumberInWindowFar(with[i], 3, window, 72, i * 16);
        UiText_DrawCharacterAtOffsetFar((s32)MsgShopStatLabel + i, window, 0, i * 16);
        UiWindow_DrawDividerLineFar(window, 0, i * 2 + 2, 13, i * 2 + 2);
    }
    if (replaced != -1)
        UiText_DrawCharacterAtOffsetFar(replaced + (s32)MsgItemName, window, 0, 48);
}

#undef gMenuWork

void RenderOutput_PrepareForRedrawFar(s32 window);
s32 Inventory_FindFar(s32 unit_id, s32 item_id);
struct RenderOutput *RenderOutput_CreateLoadedFar(s32 code, s32 layers, struct RenderInput *input, s32 x, s32 y);

#define RenderOutput_CreateLoadedFar ((struct RenderOutput * (*)(u16,s32,s32,s32,s32))RenderOutput_CreateLoadedFar)

void UiWork_PushValueSlotFar(s32 kosuu, s32 style);
#define gMenuWork (*(struct ShopRuntime *(*))&gMenuWork)

void Shop_DrawUnitItem(s32 window, s32 unit_id, s32 item_id)
{

    u8 *unit;
    s32 x;
    s32 y;
    s32 item_index;
    s32 slot;
    s32 off;
    s32 first_offset;
    s32 item_offset;
    s32 next_offset;
    u8 *icon;

    unit = (u8 *)UnitGetFar(unit_id);
    x = 8;
    y = 8;
    if (window != 0) {
        RenderOutput_PrepareForRedrawFar(window);
        slot = Inventory_FindFar(unit_id, item_id);
        if (slot != -1) {
            off = slot * 2 + 216;
            UiWork_PushValueSlotFar((*(u16 *)(unit + off) >> 11) + 1, 5);
            UiText_DrawCharacterAtOffsetFar((s32)MsgYouHave, window, 0, 0);
        } else {
            UiText_DrawCharacterAtOffsetFar((s32)MsgShopNoneInStock, window, 0, 0);
        }
        item_index = 0;
        first_offset = 216;
        if (*(u16 *)(unit + first_offset) != 0) {
            for (;;) {
                item_offset = item_index * 2 + 216;
                icon = (u8 *)RenderOutput_CreateLoadedFar(*(u16 *)(unit + item_offset), 27,
                                     window, x, y);
                icon[15] = 252;
                x += 16;
                if (item_index == 4) {
                    x = 8;
                    y += 16;
                }
                if (item_index == 9) {
                    x = 8;
                    y += 16;
                }
                item_index++;
                if (item_index > 14)
                    break;
                next_offset = item_index * 2 + 216;
                if (*(u16 *)(unit + next_offset) == 0)
                    break;
            }
        }
    }
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define ACTION_Y 120
#else
#define ACTION_Y 128
#endif

s32 Shop_SelBuyNum(s32 unit_id, s32 item_id)
{
    struct ShopRuntime *shop;
    struct BtlUnit *unit;
    struct ItemDefinition *item;
    s32 quantity;
    s32 chance;
    s32 slot;
    s32 maximum;
    s32 result;

    shop = gMenuWork;
    unit = UnitGetFar(unit_id);
    item = Item_GetDirectFar(item_id);
    result = 1;
    if (item->flags & 0x10) {
        UiMessage_ShowAndWait((s32)MsgHowManyToBuy);
        slot = Inventory_FindFar(unit_id, item_id);
        if (slot != -1) {
            quantity = (unit->inventory[slot] >> 11) + 1;
        } else {
            quantity = 0;
        }

        chance = 30;
        if (item->price != 0)
            chance = (u32)(gGameState.coins) / item->price;

        if (shop->party_action == 2) {
            maximum = Item_AdjustCounterFar(item_id, 0);
            if (chance > maximum)
                maximum = Item_AdjustCounterFar(item_id, 0);
            else
                maximum = chance;
            chance = maximum;
        }

        chance += quantity;
        if (chance > 30)
            chance = 30;

        shop->mode = 12;
        Shop_PlaceCursor(0, ACTION_Y, 0x30);
        result = Shop_SelectQuantity(quantity, chance, item->price);
    }
    return result;
}

#undef gMenuWork

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_FR)
#define QUANTITY_X 64
#define COINS_X    128
#elif defined(TBS_EDITION_ES)
#define QUANTITY_X 72
#define COINS_X    144
#else
#define QUANTITY_X 72
#define COINS_X    136
#endif

#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)

#define Shop_FillSelector ((void (*)(s32,s32,void*))Shop_FillSelector)

void AudioCommand_PlayFar(s32 cue);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void WaitFrames(s32 frames);
void UiWork_FinalizeFar(s32 window, s32 mode);

s32 Shop_SelectQuantity(s32 minimum, s32 maximum, s32 price)
{
    void *buffer;
    s32 window;
    s32 changed;
    s32 slot;
    s32 count;
    s32 result;
    struct RenderOutput *sprite;
#if !EDITION_INTERNATIONAL
    s32 msg;
#endif

    buffer = Runtime_AllocateBlock(14, 0x400);
    changed = 1;
    maximum -= minimum;
    result = -1;
    count = 0;
    window = UiWindow_CreateFar(7, 4, 23, 3, 2);
    if (window != 0) {
        slot = Resource_FindFreeEntry();
        if (slot != 96) {
            VramBlock_LoadCached(slot, 0x100, 0);
            RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 0, 0);
            sprite = RenderOutput_CreateFar(slot, 0x40004000, (struct RenderInput *)window, 32, 0);
            sprite->table.bits.index += 4;
            for (;;) {
                if (gKeyState & KEY_A) {
                    AudioCommand_PlayFar(112);
                    result = count + 1;
                    break;
                }
                if (gKeyState & KEY_B) {
                    AudioCommand_PlayFar(113);
                    result = -1;
                    break;
                }
                if (gKeysRepeat & KEY_LEFT) {
                    AudioCommand_PlayFar(111);
                    changed = 1;
                    count--;
                }
                if (gKeysRepeat & KEY_RIGHT) {
                    AudioCommand_PlayFar(111);
                    changed = 1;
                    count++;
                }
                if (changed) {
                    changed = 0;
                    count = (count + maximum) % maximum;
                    Dma_Set(Shop_QuantityTiles, buffer, 0x84000040, REG_DMA3);
                    Shop_FillSelector(30, 14, buffer);
                    Shop_FillSelector(minimum + maximum, 0, buffer);
                    Shop_FillSelector(minimum + count + 1, 10, buffer);
                    Shop_FillSelector(minimum, 2, buffer);
                    VramBlock_LoadCached(slot, 0x100, buffer);
                    result = count + 1;
#if EDITION_INTERNATIONAL
                    UiText_DrawNumberInWindowFar(result, 2, window, QUANTITY_X, 0);
                    UiText_DrawNumberInWindowFar(price * result, 6, window, QUANTITY_X + 16, 0);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgCoins, window, COINS_X, 0);
#else
                    UiText_DrawNumberInWindowFar(result, 2, window, 64, 0);
                    msg = (s32)MsgPiece;
                    UiText_DrawCharacterAtOffsetFar(msg, window, 80, 0);
                    UiText_DrawNumberInWindowFar(price * result, 6, window, 96, 0);
                    UiText_DrawCharacterAtOffsetFar(msg - 1, window, 144, 0);
#endif
                }
                WaitFrames(1);
            }
            WaitFrames(1);
            UiWork_FinalizeFar(window, 2);
        }
    }
    Runtime_ReleaseHeapBlock(14);
    return result;
}

#define Inventory_FindEquippedFar ((s32 (*)(s32,u8))Inventory_FindEquippedFar)
#define Party_AdjustSixDigitCounterBFar ((s32 (*)(s16))Party_AdjustSixDigitCounterBFar)
#define Inventory_EquipFar ((void (*)(s32,s32))Inventory_EquipFar)
#define UiIcon_PrepareObjectFar ((void (*)(void*))UiIcon_PrepareObjectFar)
#define Inventory_DiscardFar ((void (*)(s32,s32))Inventory_DiscardFar)
#define gMenuWork (*(struct ShopRuntime *(*))&gMenuWork)
void AudioCommand_PlayFar(s32);
#define UiWork_PushValueSlotFar ((void (*)(u32 unit_id, u32 mode))UiWork_PushValueSlotFar)
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

#undef gMenuWork
#undef UiWork_PushValueSlotFar

#undef Party_AdjustSixDigitCounterBFar

#define Party_AdjustSixDigitCounterBFar ((void (*)(s32))Party_AdjustSixDigitCounterBFar)
#define Item_AdjustCounterFar ((void (*)(s32,s32))Item_AdjustCounterFar)
#define gMenuWork (*(struct ShopRuntime *(*))&gMenuWork)
void UiWork_PushValueSlotFar(s32 value, s32 slot);
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

#undef gMenuWork

#define SideObject_CreateFar ((void (*)(s32,s32,s32,s32,s32,s32))SideObject_CreateFar)
#define gMenuWork (*(struct ShopRuntime *(*))&gMenuWork)
struct ObjectRuntime *Object_GetByIdFar(s32 unit_id);
void UiWork_FinalizeFar(s32 window, s32 style);

s32 Shop_ServicePrice(s32 entry_no, s32 kind)
{
    u8 value = UnitGetFar(entry_no)->level;
    s32 result = 0;

    if (kind == 0) {
        result = value * 20;
    } else if (kind == 1) {
        result = 10;
    } else if (kind == 2) {
        result = 50;
    } else if (kind == 3) {
        result = value * 10;
    }
    return result;
}

s32 Shop_CanServe(s32 entry_no, s32 kind)
{
    struct BtlUnit *entry = UnitGetFar(entry_no);
    s32 result = 0;

    if ((kind == 0 && entry->hp <= 0)
        || (kind == 1 && entry->doku != 0)
        || (kind == 2 && entry->akuryo != 0)
        || (kind == 3 && entry->restraint != 0)) {
        result = 1;
    }
    return result;
}

s32 Shop_CountUnits(void)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 active = 0;
    s32 variant = shop->party_action;
    s32 index;

    for (index = 0; index < shop->party_member_count; index++) {
        if (Shop_CanServe(shop->party_member_ids[index], variant))
            active++;
    }
    return active;
}

s32 Shop_MsgByMode(s32 value)
{
    s8 mode = gMenuWork->party_action;

    if (mode == 1) {
        value += (u32)MsgCurePoisonService - (u32)MsgReviveService;
    }
    if (mode == 2) {
        value += (u32)MsgExorcismService - (u32)MsgReviveService;
    }
    if (mode == 3) {
        value += (u32)MsgRemoveCurseService - (u32)MsgReviveService;
    }
    return value;
}

void UiMessage_ShowResolvedAndWait(s32 value)
{
    s32 no;

    no = BattleFx_GetResourceIdFar(gMenuWork->keeper_resource);
    UiWork_FinalizePendingCoreFar();
    value = Shop_MsgByMode(value);
    UiText_OpenMessageWindowFar(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}

void UiMessage_ShowResolvedAndRestoreState(s32 arg0)
{
    struct ShopRuntime *state;
    struct RenderOutput **slot;
    s32 value;
    u8 saved;

    state = gMenuWork;
    slot = &state->cursor.anchor;
    saved = (*slot)->active;
    value = BattleFx_GetResourceIdFar(state->keeper_resource);
    arg0 = Shop_MsgByMode(arg0);
    (*slot)->active = 13;
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(arg0, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    state->cursor.anchor->active = saved;
}

s32 Shop_ConfirmAct(s32 unit_id)
{
    s32 party_action = 0;
    s32 list_window = 0;
    struct ShopRuntime *shop;
    struct RenderOutput *cursor_anchor;

    Shop_InitializeCursorWork();
    shop = ((struct ShopRuntime *)gMenuWork);
    shop->party_action = list_window;

    {
        s32 shown =
            (u16)((struct AnimationObject *)Object_GetByIdFar(unit_id)->animation)->entries[0]->anim_id;
        shop->keeper_resource = shown;
    }

    list_window = UiWindow_CreateWithSideObjectFar(shop->keeper_resource, 0, 0, 0);
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    }
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(0, 0, 5, 5, 2);
        SideObject_CreateFar(2, 0, 0, list_window, -4, -4);
    }

    cursor_anchor = RenderOutput_CreateFar(
        shop->cursor_icon,
        0x40000000,
        (struct RenderInput *)list_window,
        0,
        0);
    cursor_anchor->active = 1;
    cursor_anchor->kind = 0;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    UiMessage_ShowResolvedAndWait((s32)MsgSanctumWelcome);

#if EDITION_INTERNATIONAL
    shop->money_window = UiWindow_CreateFar(16, 11, 12, 4, 2);
#else
    shop->money_window = UiWindow_CreateFar(16, 11, 11, 4, 2);
#endif
    Shop_DrawMoney();

    for (;;) {
        party_action = Menu_SelectEntry19To1cFar(party_action);
        shop->party_action = party_action;
        if (party_action == -1)
            break;

        {
            s32 base = (s32)MsgReviveService;
            s32 message = base;

            base = 0;
            UiMessage_ShowResolvedAndWait(message);
            if (Shop_CountUnits() == 0) {
                UiMessage_ShowResolvedAndWait(message + 1);
            } else {
                Sanctum_RunPartyService();
            }
        }
        shop->party_action = 0;
        ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
        UiMessage_ShowResolvedAndWait((s32)MsgSanctumMoreAid);
    }

    UiMessage_ShowResolvedAndWait((s32)MsgSanctumFarewell);
    UiWork_FinalizeFar(shop->money_window, 2);
    UiWork_FinalizeFar(list_window, 2);
    Inn_Cleanup();
    return 0;
}

s32 Sanctum_RunPartyService(void)
{
    struct ShopRuntime *shop = ((struct ShopRuntime *)gMenuWork);
    s32 price_window;
    s32 redraw;
    s32 kind;
    s32 list_window;
    s32 selection;
    s32 unit_id;
    s32 retry;
    s32 price;
    s32 message;

    price_window = 0;
    redraw = 1;
    kind = shop->party_action;
    UiMessage_ShowResolvedAndWait((s32)MsgWhoToRevive);

    list_window = UiWindow_CreateFar(1, 12, 13, 3, 2);
    shop->cursor.anchor->active = 4;
    shop->mode = redraw;
    PsynergyMenu_InitializeEntryObjectsFar(list_window, 2, 0, 8, price_window);
#if defined(TBS_EDITION_DE)
    price_window = UiWindow_CreateFar(0, 16, 30, 3, 2);
#elif defined(TBS_EDITION_FR)
    price_window = UiWindow_CreateFar(1, 16, 25, 3, 2);
#else
    price_window = UiWindow_CreateFar(1, 16, 23, 3, 2);
#endif

    selection = 0;
    unit_id = 0;
    retry = 0;
    while (selection < shop->party_member_count) {
        unit_id = shop->party_member_ids[selection];
        if (Shop_CanServe(unit_id, kind) != 0)
            break;
        selection++;
    }

    redraw = 1;
    for (;;) {
        if (retry != 0) {
            retry = 0;
            UiMessage_ShowResolvedAndWait((s32)MsgWhoToRevive);
            redraw = 1;
            selection = 0;
            while (selection < shop->party_member_count) {
                unit_id = shop->party_member_ids[selection];
                if (Shop_CanServe(unit_id, kind) != 0)
                    break;
                selection++;
            }
        }

        if (redraw != 0) {
            redraw = 0;
            selection = (selection + shop->party_member_count) % shop->party_member_count;
            unit_id = shop->party_member_ids[selection];
            Shop_PlaceCursor((void *)list_window, selection * 24 - 12, 0);
            shop->mode = 3;
            Shop_HiliteUnit(list_window, selection);
            Shop_DrawSelMsg(price_window, unit_id);
        }

        if ((gKeyState & KEY_A) != 0) {
            WaitFrames(1);
            price = Shop_ServicePrice(unit_id, kind);
            if (Shop_CanServe(unit_id, kind) == 0) {
                AudioCommand_PlayFar(SOUND_MENU_CANCEL);
                continue;
            }
            UiWork_PushValueSlotFar(unit_id, 1);
            UiWork_PushValueSlotFar(price, 5);
            message = (s32)MsgReviveDonation;
            UiMessage_ShowResolvedAndWait(message);
            if (UiMessage_ShowChoiceVariant(0) != 0) {
                UiMessage_ShowResolvedAndRestoreState(message + 2);
                retry = 1;
                continue;
            }
            if (price > (u32)gGameState.coins) {
                AudioCommand_PlayFar(SOUND_MENU_CANCEL);
                UiMessage_ShowResolvedAndRestoreState(message + 1);
                retry = 1;
                continue;
            }
            UiWork_PushValueSlotFar(unit_id, 1);
            UiMessage_ShowResolvedAndWait(message + 3);
            UiWork_FinalizePendingCoreFar();
            BattleUnit_ResetStateByMode(unit_id, kind);
            Shop_RunPartyMemberIconBurst(selection);
            Party_AdjustCoinsFar(-price);
            Shop_DrawMoney();
            UiWork_PushValueSlotFar(unit_id, 1);
            UiMessage_ShowResolvedAndWait(message + 4);
            if (Shop_CountUnits() != 0) {
                retry = 1;
                continue;
            }
            break;
        } else if ((gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(SOUND_MENU_CANCEL);
            break;
        } else {
            if ((gKeysRepeat & KEY_LEFT) != 0) {
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
                selection -= 1;
            }
            if ((gKeysRepeat & KEY_RIGHT) != 0) {
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
                selection += 1;
            }
            WaitFrames(1);
            continue;
        }
    }

    Menu_ReleaseEntryObjectsFar();
    UiWork_FinalizeFar(price_window, 2);
    UiWork_FinalizeFar(list_window, 2);
    WaitFrames(1);
    return 0;
}

#undef gMenuWork

#define gMenuWork (*(struct ShopRuntime *(*))&gMenuWork)

void UiWork_PushValueSlotFar(s32 message, s32 style);

void Vector_AddPolarOffset(s32, s32, s32 *);

void BattleUnit_ResetStateByMode(s32 id, s32 mode)
{
    struct BtlUnit *unit;

    unit = UnitGetFar(id);
    if (mode == 0) {
        unit->hp = unit->max_hp;
        UnitRatioFar(id);
    } else if (mode == 1) {
        unit->doku = 0;
    } else if (mode == 2) {
        unit->akuryo = 0;
    } else if (mode == 3) {
        s32 i;

        for (i = 0; i < 15; i++) {
            if (unit->inventory[i] & 0x200) {
                if (Item_GetDirectFar(unit->inventory[i])->flags & 1) {
                    unit->inventory[i] ^= 0x200;
                    UnitCalcFar(id);
                }
            }
        }
    }
}

void Shop_HiliteUnit(s32 enabled, s32 selected)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 index;
    s32 variant = shop->party_action;

    if (enabled != 0) {
        for (index = 0; index < shop->party_member_count; index++) {
            if (index == selected)
                AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 30);
            else
                AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 1);
            shop->party_member_scale[index] = 0x10000;
            if (!Shop_CanServe(shop->party_member_ids[index], variant))
                shop->party_member_scale[index] = 0xb333;
        }
    }
}

void Shop_DrawSelMsg(s32 target, s32 selection)
{
    s32 variant;
    s32 message;

    variant = gMenuWork->party_action;
    message = Shop_ServicePrice(selection, variant);
    if (target != 0) {
        RenderOutput_PrepareForRedrawFar(target);
        if (Shop_CanServe(selection, variant) != 0) {
            variant = (s32)MsgReviveCost;
        } else {
            variant = (s32)MsgNoHealingNeeded;
        }
        variant = Shop_MsgByMode(variant);
        UiWork_PushValueSlotFar(message, 5);
        UiText_DrawResourceFar(variant, target, 0, 0);
    }
}

void BattleFx_UpdateRadialMotion(struct EffectSlot *effect)
{
    struct FixedPointPosition position;
    s8 *state_pointer;
    s32 state;
    s32 result;

    state_pointer = &effect->state;
    state = *state_pointer;
    if (state == 0) {
        position.x = effect->origin_x;
        position.z = effect->origin_z;
        Vector_AddPolarOffset(0x280000, Random16(), (s32 *)&position);
        EffectSlot_SetPositionFar(effect, position.x, position.z);
        position.x = effect->origin_x;
        position.z = effect->origin_z;
        Vector_AddPolarOffset(0x40000, Random16(), (s32 *)&position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->max_speed = 0x20000;
        effect->acceleration = 0x6666;
        effect->flag42 = state;
        *state_pointer = *state_pointer + 1;
    } else if (state == 1) {
        result = BattleFx_HasReachedTargetFar(effect);
        if (result == 0)
            *state_pointer = result;
    } else if (state == 2) {
        if (BattleFx_HasReachedTargetFar(effect) == 0)
            BattleFx_ClearOwnedSlotFar(effect);
    }
}

#undef gMenuWork

#define UiWindow_CreateWithSideObjectFar ((s32 (*)(u16,s32,s32,s32))UiWindow_CreateWithSideObjectFar)

union PositionWord { s32 w; s16 h[2]; };

void AudioCommand_PlayFar(s32 sound_id);
void EffectSlot_SetCallbackFar(
    struct EffectSlot *effect,
    void (*callback)(struct EffectSlot *));

extern struct EventRuntime *gEventWork;
void Event_WaitTransitionFramesFar(void);
void Event_StartScreenTransitionFar(void);

enum InnMessageId {
    INN_MESSAGE_WELCOME = 0xd1c,
    INN_MESSAGE_STAY_COMPLETE,
    INN_MESSAGE_NOT_ENOUGH_COINS,
    INN_MESSAGE_GOODBYE,
    INN_MESSAGE_REST_COMPLETE,
};
void UiWork_PushValueSlotFar(s32, s32);
struct ObjectRuntime *Object_GetByIdFar(s32);

void Shop_ResetEffects(void)
{
    struct ShopRuntime *shop = (struct ShopRuntime *)gMenuWork;
    struct EffectSlot *effect = shop->effects;
    s32 count = 23;
    s8 member;

    do {
        count--;
        EffectSlot_UpdateFar((s32)effect);
        effect++;
    } while (count >= 0);
    member = shop->burst_member;
    if (member != -1)
        ObjectGroup_SetChildValueUnlessFifteenFar(
            shop->party_member_icons[member], (Random16() * 7) >> 16);
}

void Shop_RunPartyMemberIconBurst(s32 member)
{

    struct ShopRuntime *shop;
    s8 saved_kind;
    struct FixedPointPosition position;
    struct EffectSlot *effect;
    s32 callback_flags;
    s32 i;

    shop = ((struct ShopRuntime *)gMenuWork);
    saved_kind = shop->cursor.anchor->active;
    shop->burst_member = 0xff;
    shop->cursor.anchor->active = 13;
    AudioCommand_PlayFar(Shop_PartyActionCues[shop->party_action]);
    Shop_RestoreSceneTiles(0x00202108);
    AnimationObjects_SetActiveTimerStepsFar(shop->party_member_icons[member], 0);
    WaitFrames(20);
    callback_flags = 0xc80;
    Scheduler_AddOrUpdateCallback(Shop_ResetEffects, callback_flags);

    /* FAKEMATCH: the x store goes through a union with a halfword view so it may alias the member_z load, which keeps the reference schedule. */
    ((union PositionWord *)&position.x)->w = shop->party_member_x[member] << 16;
    position.z = (shop->party_member_y[member] << 16) + 0xfff40000;

    i = 0;
    effect = shop->effects;
    do {
        EffectSlot_InitializeFar(effect, 0x11c, position.x, position.z);
        EffectSlot_SetCallbackFar(effect, BattleFx_UpdateRadialMotion);
        EffectSlot_SetObjectModeFar(effect, 7);
        ObjectGroup_SetChildValueUnlessFifteenFar(
            effect->object,
            (Random16() * 7) >> 16);
        effect->scale_y = 0xb333;
        effect->scale_x = 0xb333;
        WaitFrames(3);
        if (i == 5) {
            shop->burst_member = member;
        }
        i++;
        effect++;
    } while (i <= 17);

    AudioCommand_WaitForStateByteClear();
    {
        u8 active_mode = 2;
        u8 *entry = (u8 *)&shop->effects[0].state;
        for (i = 23; i >= 0; i--) {
            if (*(s8 *)(entry + 5) != 0) {
                *entry = active_mode;
            }
            entry += sizeof(struct EffectSlot);
        }
    }

    WaitFrames(20);
    AudioCommand_PlayFar(126);
    shop->burst_member = -1;
    ObjectGroup_SetChildValueUnlessFifteenFar(shop->party_member_icons[member], 0);
    WaitFrames(20);

    {
        u8 *flag_entry = (u8 *)&shop->effects[0].active;
        struct EffectSlot *entry2 =
            shop->effects;
        for (i = 23; i >= 0; i--) {
            s32 flag = *flag_entry;

            flag <<= 24;

            flag_entry += 0x48;
            if (flag != 0) {
                BattleFx_ClearOwnedSlotFar(entry2);
            }
            entry2++;
        }
    }

    Scheduler_RemoveCallback(Shop_ResetEffects);
    AnimationObjects_SetActiveTimerStepsFar(shop->party_member_icons[member], 16);
    Shop_InitEffect();
    WaitFrames(30);
    shop->cursor.anchor->active = saved_kind;
}

#if EDITION_INTERNATIONAL
#define MESSAGE_WINDOW_ROWS 12
#else
#define MESSAGE_WINDOW_ROWS 11
#endif

s32 Inn_RoomPrice(s32 mode)
{
    struct ShopRuntime *shop = (struct ShopRuntime *)gMenuWork;
    s32 active = 0;
    s32 factor = Inn_PriceMultipliers[mode];
    s32 index;

    for (index = 0; index < shop->party_member_count; index++) {
        if (UnitGetFar(shop->party_member_ids[index])->hp != 0)
            active++;
    }
    return factor * active;
}

s32 Inn_CheckIn(s32 mode, s32 object_id)
{
    struct InnRuntimeState *state;
    struct ObjectRuntime *object;
    s32 win;
    s32 amount;
    s32 message_base;

    Shop_InitializeCursorWork();
    state = gMenuWork;
    state->active = 1;
    if (mode == 5)
        state->special_active = 1;

    object = Object_GetByIdFar(object_id);
    state->resource_id = ((struct AnimationObject *)object->animation)->entries[0]->anim_id;
    win = UiWindow_CreateWithSideObjectFar(state->resource_id, 0, 0, 0);

    amount = Inn_RoomPrice(mode);
    UiWork_PushValueSlotFar(amount, 5);
    message_base = (s32)MsgInnWelcome;
    UiMessage_ShowAndWait(message_base);
    state->window = UiWindow_CreateFar(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
    Shop_DrawMoney();

    if (UiMessage_ShowChoice(0) != 0) {
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(state->window, 2);
    } else if (amount > (u32)gGameState.coins) {
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(state->window, 2);
    } else {
        UiWork_FinalizeFar(state->window, 2);
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
        UiWork_FinalizeFar(win, 2);
        Inn_PlaySleep(amount);

        object = Object_GetByIdFar(object_id);
        state->resource_id = ((struct AnimationObject *)object->animation)->entries[0]->anim_id;
        win = UiWindow_CreateWithSideObjectFar(state->resource_id, 0, 0, 0);
        UiMessage_ShowAndWait(message_base
            + (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
    }

    UiWork_FinalizeFar(win, 2);
    Inn_Cleanup();
    return 0;
}

void Inn_PlaySleep(s32 room_price)
{
    s16 objects[8];
    s32 count;
    s32 index;
    struct BtlUnit *object;
    struct EventRuntime *state;

    count = Party_ListActiveOwnersFar(objects);
    Party_AdjustCoinsFar(-room_price);

    for (index = 0; index < count; index++) {
        object = UnitGetFar(objects[index]);
        if (object->hp != 0) {
            object->hp = object->max_hp;
            object->ep = object->max_ep;
            UnitRatioFar(objects[index]);
        }
    }

    state = gEventWork;
    state->value_1c0 = 0x209;
    state->value_1c8 = 60;
    WaitFrames(20);
    Event_FinishScreenTransitionFar();
    Event_WaitTransitionFramesFar();
    AudioCommand_PlayFar(86);
    AudioCommand_WaitForStateByteClear();
    WaitFrames(10);
    Event_StartScreenTransitionFar();
    Event_WaitTransitionFramesFar();
    WaitFrames(30);
    gEventWork->value_1c8 = 16;
}

#define gMenuWork (*(struct ShopRuntime *(*))&gMenuWork)

s32 Shop_PickUnitItem(s32 *selected_unit, s32 *selected_item)
{
    struct ShopRuntime *shop;
    struct RenderOutput *cursor_anchor;
    s32 list_window;
    s32 selected_index = 0;
    s32 redraw = 1;
    s32 unit_id = 0;
    s32 item_slot;
    s32 result = 0;

    Shop_InitializeCursorWork();
    shop = gMenuWork;
    shop->item_window = UiWindow_CreateFar(16, 12, 14, 8, 2);
    list_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
    cursor_anchor = RenderOutput_CreateFar(
        shop->cursor_icon,
        0x40000000,
        (struct RenderInput *)list_window,
        0,
        result);
    cursor_anchor->active = 4;
    cursor_anchor->kind = result;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    shop->mode = 12;
    PsynergyMenu_InitializeEntryObjectsFar(list_window, 2, 0, 8, result);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selected_index = (selected_index + shop->party_member_count) % shop->party_member_count;
            unit_id = shop->party_member_ids[selected_index];
            Shop_PlaceCursor((void *)list_window, selected_index * 24 - 12, 0);
            shop->mode = 3;
            Shop_DrawParty(list_window, selected_index, 0);
            Shop_DrawUnitGrid(shop->item_window, unit_id);
        }

        WaitFrames(1);
        if ((gKeyState & KEY_A) != 0) {
            if (Inventory_CountFar(unit_id) == 0) {
                AudioCommand_PlayFar(0x71);
                continue;
            }

            AudioCommand_PlayFar(0x70);
            item_slot = Shop_SelUse(unit_id);
            if (item_slot == -1) {
                shop->cursor.anchor->active = 4;
                shop->mode = 12;
                redraw = 1;
                continue;
            }
            *selected_unit = unit_id;
            *selected_item = item_slot;
            result = 0;
            goto done;
        }

        if ((gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(0x71);
            *selected_unit = -1;
            *selected_item = -1;
            result = -1;
            goto done;
        }

        if ((gKeysRepeat & KEY_LEFT) != 0) {
            AudioCommand_PlayFar(0x6f);
            selected_index--;
            redraw = 1;
        }
        if ((gKeysRepeat & KEY_RIGHT) != 0) {
            AudioCommand_PlayFar(0x6f);
            selected_index++;
            redraw = 1;
        }
    }

done:
    Menu_ReleaseEntryObjectsFar();
    UiWork_FinalizeFar(list_window, 2);
    UiWork_FinalizeFar(shop->item_window, 2);
    WaitFrames(1);
    Inn_Cleanup();
    return result;
}
s32 Inventory_CountFar(s32);
struct BtlUnit *UnitGetFar(s32 object_id);
#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)

s32 Shop_SelUse(s32 actor)
{
    struct ShopRuntime *shop = gMenuWork;

    s32 win2;
    s32 win1;
    struct BtlUnit *object;
    s32 selection;
    s32 redraw;
    s32 count;
    s32 result;
    s32 status;
    s32 flags;
    void *window;
    s32 x;
    s32 y;

    object = UnitGetFar(actor);
    count = 1;
    result = 0;
    redraw = 1;
#if EDITION_INTERNATIONAL
    win1 = UiWindow_CreateFar(14, 8, 16, 4, 2);
#else
    win1 = UiWindow_CreateFar(16, 8, 14, 4, 2);
#endif
    win2 = UiWindow_CreateFar(0, 5, 30, 3, 2);
    shop->cursor.anchor->active = 18;
    shop->mode = 12;
    selection = 0;

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            count = Inventory_CountFar(actor);
            if (selection > count - 1)
                selection = count - 1;

            flags = object->inventory[selection] & 0x1ff;
            window = (void *)shop->item_window;
            x = selection % 5 << 4;
            y = (selection / 5 << 4) + 8;
            Shop_PlaceCursor(window, x, y);
            shop->mode = 3;
            Shop_DrawUseItem(win1, actor, selection);
            Shop_DrawMsg(win2, flags + (s32)MsgItemPlainName);
        }

        WaitFrames(1);

        if ((gKeyState & KEY_A) != 0) {
            status = Inventory_CheckDiscardFar(actor, selection);
            if (status == 0) {
                AudioCommand_PlayFar(112);
                result = selection;
                goto exit_loop;
            } else if (status == -4) {
                UiText_OpenMessageWindowFar((s32)MsgCannotDropIt, 8, 1, 2);
            } else if (result == -3) {
                UiText_OpenMessageWindowFar((s32)MsgCannotRemoveIt, 8, 1, 2);
            }
            AudioCommand_PlayFar(113);
            while (UiWork_IsCompleteFar() == 0)
                WaitFrames(1);
            UiWork_FinalizePendingCoreFar();
            continue;
        }

        if ((gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(113);
            result = -1;
            goto exit_loop;
        }

        if ((gKeysRepeat & KEY_LEFT) != 0) {
            AudioCommand_PlayFar(111);
            selection -= 1;
            selection = (selection + count) % count;
            redraw = 1;
        }
        if ((gKeysRepeat & KEY_RIGHT) != 0) {
            AudioCommand_PlayFar(111);
            selection += 1;
            selection = (selection + count) % count;
            redraw = 1;
        }
        if ((gKeysRepeat & KEY_UP) != 0) {
            selection -= 5;
            if (selection < 0)
                selection += 15;
            while (selection >= count)
                selection -= 5;
            AudioCommand_PlayFar(111);
            redraw = 1;
        }
        if ((gKeysRepeat & KEY_DOWN) != 0) {
            selection += 5;
            if (selection >= count)
                selection -= 15;
            while (selection < 0)
                selection += 5;
            AudioCommand_PlayFar(111);
            redraw = 1;
        }
    }

exit_loop:
    UiWork_FinalizeFar(win2, 2);
    UiWork_FinalizeFar(win1, 2);
    WaitFrames(1);
    return result;
}

void Shop_DrawUseItem(s32 window, s32 unit_id, s32 item_id)
{
    struct BtlUnit *unit = UnitGetFar(unit_id);
    s32 masked = unit->inventory[item_id] & 0x1ff;
    u32 entry = unit->inventory[item_id];
    s32 mult = (entry >> 11) + 1;

    if (window != 0) {
        s32 result;

        RenderOutput_RedrawSavedRectFar(window);
        UiText_DrawCharacterAtOffsetFar(masked + (s32)MsgItemName, window, 0, 0);

        result = Inventory_CheckDiscardFar(unit_id, item_id);
        if (result == -4) {
#if EDITION_INTERNATIONAL
            UiText_DrawCharacterAtOffsetFar((s32)MsgCannotDrop, window, 0, 8);
#else
            UiText_DrawCharacterAtOffsetFar((s32)MsgCannotDrop, window, 8, 8);
#endif
        } else if (result == -3) {
#if EDITION_INTERNATIONAL
            UiText_DrawCharacterAtOffsetFar((s32)MsgCannotRemove, window, 0, 8);
#else
            UiText_DrawCharacterAtOffsetFar((s32)MsgCannotRemove, window, 8, 8);
#endif
        } else {
            s32 qty;
            s32 total;

            qty = Shop_SalePrice(unit->inventory[item_id]);
            total = mult *qty;

#if defined(TBS_EDITION_EN) || defined(TBS_EDITION_IT)
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 8, 8);
            UiText_DrawNumberInWindowFar(total, 5, window, 40, 8);
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 5, window, 80, 8);
#elif defined(TBS_EDITION_DE)
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 0, 8);
            UiText_DrawNumberInWindowFar(total, 5, window, 32, 8);
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 5, window, 72, 8);
#elif defined(TBS_EDITION_ES)
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 8, 8);
            UiText_DrawNumberInWindowFar(total, 5, window, 40, 8);
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 5, window, 88, 8);
#elif defined(TBS_EDITION_FR)
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 8, 8);
            UiText_DrawNumberInWindowFar(total, 5, window, 32, 8);
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 5, window, 72, 8);
#else
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading, window, 8, 8);
            UiText_DrawNumberInWindowFar(total, 5, window, 40, 8);
            UiText_DrawCharacterAtOffsetFar((s32)MsgPriceHeading - 1, window, 80, 8);
#endif
        }
    }
}

#undef gMenuWork
