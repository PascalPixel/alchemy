#include "shop.h"
#include "battle_runtime.h"
#include "gs1_edition.h"
#include "sound_ids.h"



s32 Func_08077028(s32, s32);
s32 Func_08077228(s32, u8);
s32 Func_08077230(s32);
s32 Func_08077298(s16);
void Func_080b0574(s32 message);
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
    replaced_slot = Func_08077228(unit_id, item->type);
    Audio_PlayCue(SOUND_SHOP_PURCHASE);
    if (added_slot < remaining) {
        do {
            added_slot = Func_08077028(unit_id, item_id);
            Func_08077230(0 - item->price);
            remaining -= 1;
            Func_08077298(item->price);
            Shop_DrawMoney();
        } while (remaining != 0);
    }
    Func_080b0574((s32)&Value_00000ca1);
    if (Shop_ConfirmEquipItem(unit_id, added_slot) != 0) {
        Shop_SellReplacedItem(unit_id, replaced_slot);
    }
}





s32 Func_08077218(s32 unit_id, s32 item_id);
s32 Func_08077228(s32 unit_id, u8 kind);
void Func_08077050(s32 unit_id, s32 slot);
void Func_08015120(u32 unit_id, u32 mode);
void Func_080b04dc(s32 message);
s32 Func_080b0634(s32 value);
void Func_080b1dec(s32 value, s32 unit_id);
void Func_080f9010(s32 cue);
void Func_08015140(void);
s32 Func_08015038(s32 a, s32 b, s32 c, s32 d);
s32 Func_08015048(void);
void WaitFrames(s32 frames);

extern struct ShopRuntime *Data_03001f2c;
extern char Value_00000ca2;
extern char Value_00000ca3;
extern u8 Value_00000ad0[];

s32 Shop_ConfirmEquip(s32 unit_id, s32 slot)
{
    struct ShopRuntime *menu = Data_03001f2c;
    u8 *unit = (u8 *)BattleUnit_Get(unit_id);
    s32 slot_offset = slot * 2 + 216;
    s32 masked = *(volatile u16 *)(unit + slot_offset) & 0x1ff;
    struct ItemDefinition *info = Item_Get(masked);
    s32 replaced;
    s32 menu_value;

    if (*(volatile u16 *)(unit + slot_offset) & 0x200)
        return 0;

    if (Func_08077218(unit_id, masked) == 0)
        return 0;

    replaced = Func_08077228(unit_id, info->type);
    if (replaced != -1) {
        s32 old_offset = replaced * 2 + 216;
        u16 old_raw = *(u16 *)(unit + old_offset);
        struct ItemDefinition *old_info = Item_Get(old_raw);

        if (old_info->flags & 2)
            return 0;
    }

    Func_08015120(unit_id, 1);
    Func_080b04dc((s32)&Value_00000ca2);
    if (Func_080b0634(0) != 0)
        return 0;

    Func_08077050(unit_id, slot);
    menu_value = menu->item_window;
    if (menu_value != 0)
        Func_080b1dec(menu_value, unit_id);

    if (info->flags & 1) {
        Func_080f9010(103);
        Func_08015140();
        Func_08015038((s32)Value_00000ad0, 8, 4, 2);
        while (Func_08015048() == 0) {
            WaitFrames(1);
        }
    }

    Func_080b0574((s32)&Value_00000ca3);
    return 1;
}




void Func_080b1f4c(s32, s32, s32);

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
    Func_080b1f4c(unit_id, slot, -1);
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





#if defined(GS1_EDITION_JA)
#define BASE_W 11
#else
#define BASE_W 12
#endif

s32 Modulo(s32, s32);
void WaitFrames(s32);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
void UiWindow_Close(s32, s32);
s32 Ability_GetAvailability(s32);
void Func_080a1028(s32, s32, s32, s32, s32);
void Func_080a1030(void);
s32 Func_080b1bd0(s32);
void Func_080b211c(s32);

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
    Func_080a1028(list_window, 2, 0, 8, 0);

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
            Shop_UpdatePartyMemberList(list_window, selection, 0);
            Shop_DrawPartyMemberItemGrid(shop->item_window, unit_id);
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            WaitFrames(1);
            if (Ability_GetAvailability(unit_id) == 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
            } else {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
                if (shop->party_action == 1)
                    Func_080b1bd0(unit_id);
                else
                    Func_080b211c(unit_id);
                shop->cursor.anchor->kind = 4;
                shop->mode = 12;
                redraw = 1;
            }
            continue;
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
            Audio_PlayCue(SOUND_MENU_CANCEL);
            Func_080a1030();
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





#define Shop_SelSell Func_080b1bd0

s32 FixedPoint_Ratio(s32, s32);
void UiMessage_ShowAndWait(s32);

extern u8 Value_00000075;
extern u8 Value_00000caa;

/*
 * Sell flow reached from Shop_SelectPartyMember when the shop's party action
 * is "sell": browse the chosen member's inventory, priced one slot at a
 * time, and hand a confirmed slot off to Shop_SelectSaleQuantity before
 * writing the sale back through Func_080b1f4c.
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
                    Shop_ComputeSalePrice(unit->inventory[selection]),
                    1);
                Shop_DrawMessage(price_window, item_id + (s32)&Value_00000075);
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

        quantity = Shop_SelectSaleQuantity(unit_id, selection);
        if (quantity != -1)
            Func_080b1f4c(unit_id, selection, quantity);
        UiMessage_ShowAndWait((s32)&Value_00000caa);
        if (Ability_GetAvailability(unit_id) == 0)
            break;
    }

    UiWindow_Close(list_window, 2);
    return result;
}




extern u8 Value_00000c91;

void UiWindow_Clear(s32 window);
s32 Ability_GetAvailability(s32 unit_id);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
u8 *UiIcon_Draw(u16 no, s32 kind, s32 window, s32 x, s32 y);

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





#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_FR)
#define EFFECT_X 0x78
#else
#define EFFECT_X 0x80
#endif

s32 Shop_GetSelectionState(s32, s32);
void Func_080b04dc(s32);
s32 Func_080b1614(s32, s32, s32);
void Func_080a1038(void *);
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
    effect = Shop_ComputeSalePrice(*(u16 *)((u8 *)unit + entry_offset));
    state = Shop_GetSelectionState(unit_id, slot);
    selection = state;
    if ((item->flags & 0x10) && state > 1) {
        Func_080b04dc((s32)&Value_00000cad);
        saved_x = shop->cursor.target_x;
        saved_y = shop->cursor.target_y;
        shop->cursor.anchor->kind = 4;
        shop->mode = 0xc;
        Shop_PlaceCursor(NULL, EFFECT_X, 0x30);
        result = Func_080b1614(0, selection, effect);
        WaitFrames(1);
        Func_080a1038(shop->cursor.anchor);
        Shop_PlaceCursor(NULL, saved_x, saved_y);
    }
    return result;
}
