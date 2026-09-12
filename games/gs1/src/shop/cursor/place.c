#include "scene.h"
#include "shop.h"
#include "global_cells.h"
#include "far_runtime.h"
#include "battle_runtime.h"
#include "gs1_edition.h"
#include "sound_ids.h"
#include "types.h"

/* shop/place_cursor.c */
/* shop/cursor/place.c */
void Shop_PlaceCursor(void *window, s32 x, s32 y)
{
    s32 cursor_x;
    s32 cursor_y;
    struct ShopRuntime *shop;

    cursor_x = x;
    cursor_y = y;
    shop = SHOP_RUNTIME;
    if (window != NULL) {
        cursor_x = cursor_x + (FIELD_AT_OFFSET(window, u16 *, 0xC) * 8) + 8;
        cursor_y = cursor_y + (FIELD_AT_OFFSET(window, u16 *, 0xE) * 8) + 8;
    }
    Shop_SetCursor(
        &shop->cursor,
        cursor_x,
        cursor_y,
        (s8)shop->mode);
}

/* shop/draw/stock.c */
s32 Modulo(s32 value, s32 divisor);
void UiWindow_Clear(s32 window);
u8 *UiIcon_DrawWithFlags(u16 no, u32 flags, s32 window, s32 x, s32 y);
u8 *UiIcon_Draw(s32 no, s32 kind, s32 window, s32 x, s32 y);
struct BattleActorDefinition *FunctionHead_08077008(s32 actor_id);

void Shop_DrawStock(s32 window, s32 selected)
{
    struct ShopRuntime *shop;
    s16 *item_ids;
    s16 *item_id;
    s32 item_count;
    s32 first;
    s32 slot;
    s32 x;
    s32 clear;
    s32 highlight;
    s16 stock_item;
    struct ItemDefinition *definition;
    u8 *icon;

    shop = SHOP_RUNTIME;
    item_ids = shop->stock_item_ids;
    item_count = shop->stock_count;
    first = selected - Modulo(selected, 7);
    if (window != 0) {
        UiWindow_Clear(window);
        if (first != 0) {
            icon = UiIcon_DrawWithFlags(shop->previous_page_icon, 0x40000000,
                                 window, 216, -16);
            clear = 0;
            icon[4] = clear;
            icon[5] = 17;
            *(u16 *)(icon + 12) = clear;
        }
        if (first + 7 < item_count) {
            icon = UiIcon_DrawWithFlags(shop->next_page_icon, 0x40000000,
                                 window, 216, 24);
            clear = 0;
            icon[4] = clear;
            icon[5] = 15;
            *(u16 *)(icon + 12) = clear;
        }
        slot = 0;
        if ((u32)first < (u32)item_count) {
            item_id = (s16 *)((u32)(first * 2) + (u32)item_ids);
            x = 16;
            for (; (u32)slot <= 6 && (u32)first < (u32)item_count;
                 x += 32, slot++, item_id++, first++) {
                stock_item = *item_id;
                definition = Item_Get(stock_item);
                icon = UiIcon_Draw(
                    stock_item, 1, window, slot * 32, 0);
                icon[15] = 252;
                if (first == selected) {
                    icon[5] = 9;
                    highlight = 10;
                    *(u16 *)(icon + 12) = highlight;
                    icon[15] = 253;
                }
                icon = FunctionHead_080b0744(definition->price, window, x, 0);
                icon[15] = 251;
            }
        }
    }
}

/* shop/draw/money.c */
void UiText_DrawAt(s32 message, s32 window, s32 x, s32 y);
s32 UiNumber_DrawAt(s32, s32, s32, s32, s32);
void Shop_DrawMoney(void)
{
    struct ShopRuntime *shop;
    s32 window;

    shop = SHOP_RUNTIME;
    window = shop->money_window;
    if (window != 0) {
        UiText_DrawAt(0xc8a, window, 0, 0);
        UiNumber_DrawAt(SHOP_PARTY_STATE.money, 6, window, 0x20, 8);
    }
}

/* shop/draw/msg.c */

void Shop_DrawMsg(s32 window, s32 message)
{
    if (window != 0) {
        UiWindow_Commit(window);
        UiText_DrawAt(message, window, 0, 0);
    }
}

/* shop/draw/party.c */
void Object_InitializeMode(void *, s32);

extern struct ShopMenuState_080b1868 *gIw;

union ShopPartyMemberId {
    s32 word;
    s16 half[2];
};

void Shop_DrawParty(s32 window, s32 selected, s32 requirement)
{
    u8 *shop;
    u8 *party_member_base;
    s32 *icon_entry;
    s32 index;
    s32 offset;
    s16 unit_id;

    shop = gIw;
    if (window != 0) {
        index = 0;
        if (index < *(s8 *)(shop + 0x3a7)) {
            party_member_base = shop + 2;
            offset = 0x36c;
            icon_entry = (s32 *)(shop + 0x114);
            do {
                if (index == selected)
                    Object_InitializeMode((void *)*icon_entry, 30);
                else
                    Object_InitializeMode((void *)*icon_entry, 1);
                icon_entry[16] = 0x10000;
                unit_id = ((union ShopPartyMemberId *)(
                    party_member_base + offset))->half[0];
                if (FunctionHead_08077220(unit_id, requirement) == 0)
                    icon_entry[16] = 0xcccc;
                index++;
                offset += 2;
                icon_entry++;
            } while (index < *(s8 *)(shop + 0x3a7));
        }
    }
}

/* shop/draw/unit_item.c */
/* 所持品欄の再描画。窓を開き直し、選択中の品目に応じて見出しを差し替え、
   所持枠を左上から順に並べる。枠は5個目と10個目で折り返す。
   枠番号は0xd8からのu16列で、0が終端。 */

extern u8 Value_00000c90;
extern u8 Value_00000c8f;

s32 Item_FindSlot(s32 unit_id, s32 item_id);
void UiText_DrawQuantity(s32 kosuu, s32 style);

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

    unit = (u8 *)BattleUnit_Get(unit_id);
    x = 8;
    y = 8;
    if (window != 0) {
        UiWindow_Clear(window);
        slot = Item_FindSlot(unit_id, item_id);
        /* 参照は枠位置を「バイト差」として先に組み、状態先頭を基底に残す。
           足し込む順を変えると二レジスタ番地形が崩れる。 */
        if (slot != -1) {
            off = slot * 2 + 216;
            UiText_DrawQuantity((*(u16 *)(unit + off) >> 11) + 1, 5);
            UiText_DrawAt((s32)&Value_00000c90, window, 0, 0);
        } else {
            UiText_DrawAt((s32)&Value_00000c8f, window, 0, 0);
        }
        item_index = 0;
        first_offset = 216;
        if (*(u16 *)(unit + first_offset) != 0) {
            for (;;) {
                item_offset = item_index * 2 + 216;
                icon = UiIcon_Draw(*(u16 *)(unit + item_offset), 27,
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

/* shop/sel/buy_num.c */
#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_FR)
#define ACTION_Y 120
#else
#define ACTION_Y 128
#endif

extern u8 Value_00000ca0;


s32 Ability_GetMaximum(s32 value, s32 enabled);

s32 Shop_SelBuyNum(s32 unit_id, s32 item_id)
{
    struct ShopRuntime *shop;
    struct BattleUnit *unit;
    struct ItemDefinition *item;
    s32 quantity;
    s32 chance;
    s32 slot;
    s32 maximum;
    s32 result;

    shop = SHOP_RUNTIME;
    unit = BattleUnit_Get(unit_id);
    item = Item_Get(item_id);
    result = 1;
    if (item->flags & 0x10) {
        FunctionHead_080b04dc((s32)&Value_00000ca0);
        slot = Item_FindSlot(unit_id, item_id);
        if (slot != -1) {
            quantity = (unit->inventory[slot] >> 11) + 1;
        } else {
            quantity = 0;
        }

        chance = 30;
        if (item->price != 0)
            chance = FunctionHead_080022f4(SHOP_PARTY_STATE.money, item->price);

        if (shop->party_action == 2) {
            maximum = Ability_GetMaximum(item_id, 0);
            if (chance > maximum)
                maximum = Ability_GetMaximum(item_id, 0);
            else
                maximum = chance;
            chance = maximum;
        }

        chance += quantity;
        if (chance > 30)
            chance = 30;

        shop->mode = 12;
        Shop_PlaceCursor(0, ACTION_Y, 0x30);
        result = FunctionHead_080b1614(quantity, chance, item->price);
    }
    return result;
}

/* shop/sel/buy_done.c */
void Audio_PlayCue(s32);
extern char Value_00000ca1;

void Shop_BuyDone(s32 unit_id, s32 item_id, s32 quantity)
{
    s32 replaced_slot;
    s32 remaining;
    s32 added_slot;
    struct ItemDefinition *item;

    remaining = quantity;
    item = Item_Get(item_id);
    added_slot = 0;
    replaced_slot = FunctionHead_08077228(unit_id, item->type);
    Audio_PlayCue(SOUND_SHOP_PURCHASE);
    if (added_slot < remaining) {
        do {
            added_slot = FunctionHead_08077028(unit_id, item_id);
            FunctionHead_08077230(0 - item->price);
            remaining -= 1;
            Party_AdjustSixDigitCounterB(item->price);
            Shop_DrawMoney();
        } while (remaining != 0);
    }
    FunctionHead_080b0574((s32)&Value_00000ca1);
    if (Shop_ConfirmEquip(unit_id, added_slot) != 0) {
        Shop_SellOld(unit_id, replaced_slot);
    }
}

/* shop/sel/equip.c */
struct ShopMenuState_080b1868 {
    u8 unknown_00[0x20];
    s32 value_20;
};


void WaitFrames(s32);

extern char Value_00000ca2;
extern char Value_00000ca3;
extern u8 Value_00000ad0[];

s32 Shop_ConfirmEquip(s32 unit_id, s32 slot)
{
    struct ShopMenuState_080b1868 *menu = gIw;
    u8 *unit = (u8 *)FunctionHead_08077008(unit_id);
    s32 slot_offset = slot * 2 + 216;
    s32 masked = *(volatile u16 *)(unit + slot_offset) & 0x1ff;
    struct ItemDefinition *info = Item_Get(masked);
    s32 replaced;
    s32 menu_value;

    if (*(volatile u16 *)(unit + slot_offset) & 0x200)
        return 0;

    if (FunctionHead_08077218(unit_id, masked) == 0)
        return 0;

    replaced = FunctionHead_08077228(unit_id, info->type);
    if (replaced != -1) {
        s32 old_offset = replaced * 2 + 216;
        u16 old_raw = *(u16 *)(unit + old_offset);
        struct ItemDefinition *old_info = Item_Get(old_raw);

        if (old_info->flags & 2)
            return 0;
    }

    UiText_DrawQuantity(unit_id, 1);
    FunctionHead_080b1bd0((s32)&Value_00000ca2);
    if (FunctionHead_080b0634(0) != 0)
        return 0;

    Sys_Apply4(unit_id, slot);
    menu_value = menu->value_20;
    if (menu_value != 0)
        Sys_Apply5(menu_value, unit_id);

    if (info->flags & 1) {
        FunctionHead_080f9010(103);
        UiWork_FinalizePending();
        UiWork_Create((s32)Value_00000ad0, 8, 4, 2);
        while (UiWork_IsCompleteFar() == 0) {
            WaitFrames(1);
        }
    }

    UiMessage_ShowAndRestoreState((s32)&Value_00000ca3);
    return 1;
}

/* shop/sel/sell_old.c */
s32 Shop_SellOld(s32 unit_id, s32 slot)
{
    struct BattleUnit *unit;
    s32 item_offset;
    s32 item_id;

    unit = BattleUnit_Get(unit_id);
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
    FunctionHead_080b1f4c(unit_id, slot, -1);
    return 1;
}

/* shop/price/sale.c */
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

/* shop/sel/pick_unit.c */
#if defined(GS1_EDITION_JA)
#define BASE_W 11
#else
#define BASE_W 12
#endif

s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void UiWindow_Close(s32 window, s32 style);
s32 Ability_GetAvailability(s32 unit_id);


/*
 * Keep an actor-selection menu active while dispatching the chosen actor into
 * one of two action screens.  The menu itself closes only when cancelled.
 */
s32 Shop_PickUnit(void)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
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
    Sys_SetRange(list_window, 2, 0, 8, 0);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selection = Modulo(
                selection + shop->party_member_count,
                shop->party_member_count);
            unit_id = shop->party_member_ids[selection];
            Shop_PlaceCursor(
                (void *)list_window,
                selection * 24 - 12,
                0);
            shop->mode = 3;
            Shop_DrawParty(list_window, selection, 0);
            Shop_DrawUnitGrid(shop->item_window, unit_id);
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            WaitFrames(1);
            if (Ability_GetAvailability(unit_id) == 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
            } else {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
                if (shop->party_action == 1)
                    FunctionHead_080b1bd0(unit_id);
                else
                    FunctionHead_080b211c(unit_id);
                shop->cursor.anchor->kind = 4;
                shop->mode = 12;
                redraw = 1;
            }
            continue;
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
            Audio_PlayCue(SOUND_MENU_CANCEL);
            FunctionHead_080a1030();
            UiWindow_Close(list_window, 2);
            UiWindow_Close(shop->item_window, 2);
            UiWindow_Close(shop->money_window, 2);
            WaitFrames(1);
            return 0;
        }

        if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            selection--;
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            selection++;
            redraw = 1;
        }
        WaitFrames(1);
    }
}

/* shop/sel/sell.c */
s32 FixedPoint_Ratio(s32, s32);
void UiMessage_ShowAndWait(s32);

extern u8 Value_00000075;
extern u8 Value_00000caa;

/*
 * Sell flow reached from Shop_PickUnit when the shop's party action
 * is "sell": browse the chosen member's inventory, priced one slot at a
 * time, and hand a confirmed slot off to Shop_SelSellNum before
 * writing the sale back through Sys_Place.
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
                Shop_PlaceCursor(window, x, FixedPoint_Ratio(selection, 5) * 16 + 8);
                shop->mode = 3;
                Shop_DrawItemPrice(
                    list_window,
                    item_id,
                    Shop_SalePrice(unit->inventory[selection]),
                    1);
                Shop_DrawMsg(price_window, item_id + (s32)&Value_00000075);
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
                selection = Modulo(selection + item_count, item_count);
                redraw = 1;
            }
            if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                selection += 1;
                selection = Modulo(selection + item_count, item_count);
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
        UiWindow_Close(price_window, 2);
        WaitFrames(1);
        if (result != 0)
            break;

        quantity = Shop_SelSellNum(unit_id, selection);
        if (quantity != -1)
            FunctionHead_080b1f4c(unit_id, selection, quantity);
        UiMessage_ShowAndWait((s32)&Value_00000caa);
        if (Ability_GetAvailability(unit_id) == 0)
            break;
    }

    UiWindow_Close(list_window, 2);
    return result;
}

/* shop/draw/unit_grid.c */
extern u8 Value_00000c91;

void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);

void Shop_DrawUnitGrid(s32 window, s32 unit_id)
{
    u8 *unit;
    s32 x;
    s32 y;
    s32 slot;
    s32 item_offset;
    u8 *icon;

    unit = (u8 *)BattleUnit_Get(unit_id);
    x = 8;
    y = 0;
    if (window != 0) {
        UiWindow_Clear(window);
        if (Ability_GetAvailability(unit_id) == 0) {
            UiText_DrawMessageAt((s32)&Value_00000c91, window, 8, 20);
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

/* shop/sel/sell_num.c */
#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_FR)
#define EFFECT_X 0x78
#else
#define EFFECT_X 0x80
#endif

s32 Shop_GetSelectionState(s32, s32);


extern char Value_00000cad;

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

    shop = SHOP_RUNTIME;
    unit = BattleUnit_Get(unit_id);
    entry_offset = (slot * 2) + 0xd8;
    item = Item_Get(*(u16 *)((u8 *)unit + entry_offset));
    result = 1;
    effect = Shop_SalePrice(*(u16 *)((u8 *)unit + entry_offset));
    state = Shop_GetSelectionState(unit_id, slot);
    selection = state;
    if ((item->flags & 0x10) && state > 1) {
        FunctionHead_080b1bd0((s32)&Value_00000cad);
        saved_x = shop->cursor.target_x;
        saved_y = shop->cursor.target_y;
        shop->cursor.anchor->kind = 4;
        shop->mode = 0xc;
        Shop_PlaceCursor(NULL, EFFECT_X, 0x30);
        result = FunctionHead_080b1f4c(0, selection, effect);
        WaitFrames(1);
        Shop_SelRepair(shop->cursor.anchor);
        Shop_PlaceCursor(NULL, saved_x, saved_y);
    }
    return result;
}

/* shop/price/repair.c */
s32 Shop_RepairPrice(s32 item_id)
{
    s32 result = Item_Get(item_id)->price / 4;

    if ((item_id & 0x400) == 0) {
        result = 0;
    }
    return result;
}

/* shop/sel/unit.c */
s32 Shop_SelUnit(void)
{
    Shop_PickUnit();
    return 0;
}

/* shop/sel/repair.c */

extern u8 Value_00000cc2;

/*
 * Repair flow reached from Shop_PickUnit when the shop's party
 * action is not "sell": browse the chosen member's inventory, priced one
 * slot at a time, and hand a confirmed slot off to Sys_Apply before
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
                item_count = Ability_GetAvailability(unit_id);
                if (selection > item_count - 1)
                    selection = item_count - 1;
                item_id = 0x1ff & unit->inventory[selection];
                window = (void *)shop->item_window;
                x = Modulo(selection, 5) * 16;
                y = FixedPoint_Ratio(selection, 5) * 16 + 8;
                Shop_PlaceCursor(window, x, y);
                shop->mode = 3;
                price = Shop_RepairPrice(unit->inventory[selection]);
                Shop_DrawItemPrice(list_window, item_id, price, 2);
                Shop_DrawMsg(
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
                selection = Modulo(selection + item_count, item_count);
                redraw = 1;
            }
            if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                selection += 1;
                selection = Modulo(selection + item_count, item_count);
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

        UiWindow_Close(price_window, 2);
        WaitFrames(1);
        if (result != 0)
            break;

        FunctionHead_080b2328(unit_id, selection);
        UiMessage_ShowAndWait((s32)&Value_00000cc2);
        if (Ability_GetAvailability(unit_id) == 0)
            break;
    }

    UiWindow_Close(list_window, 2);
    return result;
}

/* field/event_table/get_row_limit.c */
/* field/event_table/get_row_limit.c */

int EventTable_GetRowLimit(void)
{
    return 35;
}

/* field/event_table/apply_row_abilities.c */
extern s16 gRom[][33];

s32 GameFlag_IsSet(s32 flag_no);
void GameFlag_Set(s32 flag_no);

void EventTable_ApplyRowAbilities(s32 row_no)
{
    s16 *row;
    s16 *entry;
    s32 value;
    s32 count;
    s32 flag;

    flag = row_no + 0x400;
    if (GameFlag_IsSet(flag) == 0) {
        GameFlag_Set(flag);
        count = 0;
        value = gRom[row_no][24];
        if (value != 0) {
            row = gRom[row_no];
            entry = row + 24;
            do {
                Ability_GetMaximum(value, 1);
                count++;
                if (count > 7) {
                    break;
                }
                entry++;
                value = *entry;
            } while (value != 0);
        }
    }
}

/* field/event_table/get_row_type.c */
struct EventTable {
    s16 header[32];
    s16 entries[1][33];
};

extern struct EventTable gRom;

s32 EventTable_GetRowType(s32 index)
{
    return gRom.entries[index][0];
}

/* shop/service_price.c */
/* shop/service_price_2.c */
/* shop/service_price.c */
/* shop/price/service.c */
u8 *Runtime_GetObject(s32);

s32 Shop_ServicePrice(s32 unused, s32 kind)
{
    u8 value = Runtime_GetObject()[0xF];
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

/* shop/unit/can_serve.c */

s32 Shop_CanServe(s32 entry_no, s32 kind)
{
    u8 *entry = Runtime_GetObject(entry_no);
    s32 result = 0;

    if ((kind == 0 && *(s16 *)(entry + 56) <= 0)
        || (kind == 1 && *(s8 *)(entry + 305) != 0)
        || (kind == 2 && entry[320] != 0)
        || (kind == 3 && *(s8 *)(entry + 304) != 0)) {
        result = 1;
    }
    return result;
}

/* shop/unit/count.c */

s32 Shop_CountUnits(void)
{
    u8 *work = gIw;
    u8 *base;
    s32 active = 0;
    s32 variant = (s8)work[0x3AA];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(work + 0x3A7)) {
        base = work + 2;
        offset = 0x36C;
        do {
            if (Sys_ApplyServicePrice(*(s16 *)(base + offset), variant) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(work + 0x3A7));
    }

    return active;
}

/* shop/draw/msg_mode.c */
struct Work_080b2884 {
    u8 padding[0x3AA];
    s8 mode;
};

extern u8 Value_00000d24;
extern u8 Value_00000d2e;
extern u8 Value_00000d38;
extern u8 Value_00000d42;

s32 Shop_MsgByMode(s32 value)
{
    s8 mode = gIw->mode;

    if (mode == 1) {
        value += (u32)&Value_00000d2e - (u32)&Value_00000d24;
    }
    if (mode == 2) {
        value += (u32)&Value_00000d38 - (u32)&Value_00000d24;
    }
    if (mode == 3) {
        value += (u32)&Value_00000d42 - (u32)&Value_00000d24;
    }
    return value;
}

/* ui/message/message_show_resolved_and_wait.c */
/* ui/message/show_resolved_and_wait.c */
struct Work_080b28d4 {
    u8 unknown_000[0x3a4];
    u16 value_3a4;
};

void UiWork_FinalizePending(void);

void UiWork_Create(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);


void UiMessage_ShowResolvedAndWait(s32 value)
{
    s32 no;

    no = Ui_Check(gIw->value_3a4);
    UiWork_FinalizePending();
    value = Shop_MsgByMode(value);
    UiWork_Create(value, 5, 0, (no << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
}

/* ui/message/show_resolved_and_restore_state.c */



struct State080b2928 {
    u8 padding0[0x380];
    void *mode_state;
    u8 padding1[0x20];
    u16 value;
};

void UiMessage_ShowResolvedAndRestoreState(s32 arg0)
{
    struct State080b2928 *state;
    void **slot;
    s32 value;
    u8 saved;

    state = gIw;
    slot = &state->mode_state;
    saved = *(u8 *)((u8 *)*slot + 5);
    value = Ui_Check(state->value);
    arg0 = Shop_MsgByMode(arg0);
    *(u8 *)((u8 *)*slot + 5) = 13;
    UiWork_FinalizePending();
    UiWork_Create(arg0, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
    *(u8 *)((u8 *)state->mode_state + 5) = saved;
}

/* shop/sel/confirm.c */

struct ShopCursorAnchor *UiIcon_DrawWithFlags(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);


/* Run the shop's yes/no party-action confirmation prompt for one unit. */
s32 Shop_ConfirmAct(s32 unit_id)
{
    s32 party_action = 0;
    s32 list_window = 0;
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;

    Sys_RunRoomPrice();
    shop = SHOP_RUNTIME;
    shop->party_action = list_window;

    {
        s32 shown =
            *(u16 *)(*(u32 *)(*(u32 *)((u8 *)Scene_GetRecord(unit_id) + 80) + 40));
        *(u16 *)((u8 *)shop + 0x3a4) = shown;
    }

    list_window = Sys_SetModeServicePrice(*(u16 *)((u8 *)shop + 0x3a4), 0, 0, 0);
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    }
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(0, 0, 5, 5, 2);
        Sys_SetRect(2, 0, 0, list_window, -4, -4);
    }

    cursor_anchor = UiIcon_DrawWithFlags(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        0);
    cursor_anchor->kind = 1;
    cursor_anchor->unknown_00[4] = 0;
    Sys_PlaceServicePrice(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    Sys_DoServicePrice(0xd21);

    shop->money_window = UiWindow_CreateFar(16, 11, 12, 4, 2);
    Shop_DrawMoney();

    for (;;) {
        party_action = Menu_SelectEntry19To1c(party_action);
        shop->party_action = party_action;
        if (party_action == -1)
            break;

        {
            s32 base = 0xd24;
            s32 message = base;

            base = 0;
            Sys_DoServicePrice(message);
            if (Shop_CountUnits() == 0) {
                Sys_DoServicePrice(message + 1);
            } else {
                Shop_SelectTreatmentMember();
            }
        }
        shop->party_action = 0;
        Sys_PlaceServicePrice(&shop->cursor, -32, 112);
        Sys_DoServicePrice(0xd22);
    }

    Sys_DoServicePrice(0xd23);
    UiWindow_Close(shop->money_window, 2);
    UiWindow_Close(list_window, 2);
    Inn_Cleanup();
    return 0;
}
