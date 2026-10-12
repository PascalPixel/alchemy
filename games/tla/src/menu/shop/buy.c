#include "types.h"
#include "item.h"
#include "ram_buffer.h"

s32 Inventory_AddItemFar(s32 unit_id, s32 item_id);
s32 Inventory_FindEquippedFar(s32 unit_id, s32 type);
s32 Party_AdjustCoinsFar(s32 amount);
s32 Party_AdjustSixDigitCounterBFar(s32 amount);
extern u8 MsgHereYouGo[];
void Audio_PlayCue(s32 cue);
void Func_0810857c(s32 message_id);
void Func_08109188(void);
s32 Shop_ConfirmEquip(s32 unit_id, s32 slot);
s32 Shop_SellOld(s32 unit_id, s32 slot);

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
    Audio_PlayCue(101);
    if (added_slot < remaining) {
        do {
            added_slot = Inventory_AddItemFar(unit_id, item_id);
            Party_AdjustCoinsFar(0 - (u16)item->price);
            remaining -= 1;
            Party_AdjustSixDigitCounterBFar((u16)item->price);
            Func_08109188();
        } while (remaining != 0);
    }
    Func_0810857c((s32)MsgHereYouGo);
    if (Shop_ConfirmEquip(unit_id, added_slot) != 0)
        Shop_SellOld(unit_id, replaced_slot);
}

struct ShopUnit {
    u8 unknown_00[216];
    u16 inventory[15];
};

struct ShopWork {
    u8 unknown_00[0x24];
    void *item_window;
};

void *UnitGet(s32 unit_id);
void Inventory_EquipFar(s32 unit_id, s32 slot);
void UiText_DrawQuantity(s32 value, s32 slot);
void Func_081084f4(s32 message_id);
s32 Func_08108630(s32 choice);
void Shop_DrawItemCount(void *window, s32 item);
void UiWork_FinalizePendingCoreFar(void);
void UiText_OpenMessageWindowFar(s32 message_id, s32 x, s32 y, s32 flags);
s32 UiWork_IsCompleteFar(void);
void WaitFrames(s32 frames);
extern u8 MsgEquipNowPrompt[];
extern u8 MsgBecameCursed[];
extern u8 MsgLookBolder[];

s32 Shop_ConfirmEquip(s32 unit_id, s32 slot)
{
    struct ShopWork *shop = Ram_HeapSlots->menu_runtime;
    struct ShopUnit *unit = UnitGet(unit_id);
    u32 raw = unit->inventory[slot];
    s32 item_id;
    struct ItemDefinition *info;
    s32 replaced;
    void *window;

    /* FAKEMATCH: the barrier keeps the slot halfword load ahead of the 0x1ff mask; without it the mask is started first (one reordered pair, differ 2). */
    asm volatile("");
    item_id = 0x1ff;
    item_id &= raw;
    info = Item_Get(item_id);

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

    UiText_DrawQuantity(unit_id, 1);
    Func_081084f4((s32)MsgEquipNowPrompt);
    if (Func_08108630(0) != 0)
        return 0;

    Inventory_EquipFar(unit_id, slot);
    window = shop->item_window;
    if (window != 0)
        Shop_DrawItemCount(window, unit_id);

    if (info->flags & 1) {
        Audio_PlayCue(103);
        UiWork_FinalizePendingCoreFar();
        {
            s32 x = 8;
            s32 y = 4;
            s32 flags = 2;

            /* FAKEMATCH: the native loads the three small arguments before the message; the plain call loads the message first (one instruction moved, differ 2). */
            asm volatile("" : "+l"(x), "+l"(y), "+l"(flags));
            UiText_OpenMessageWindowFar((s32)MsgBecameCursed, x, y, flags);
        }
        while (UiWork_IsCompleteFar() == 0)
            WaitFrames(1);
    }

    Func_0810857c((s32)MsgLookBolder);
    return 1;
}

void Func_0810a108(s32 unit_id, s32 slot, s32 count);

s32 Shop_SellOld(s32 unit_id, s32 slot)
{
    u8 *unit;
    s32 item_offset;
    s32 item_id;
    u32 raw;

    unit = UnitGet(unit_id);
    if (slot == -1)
        return 0;
    item_offset = slot * 2 + 216;
    raw = *(u16 *)(unit + item_offset);
    /* FAKEMATCH: the barrier keeps the slot halfword load ahead of the 0x1ff mask; without it the mask is started first (one reordered pair, differ 2). */
    asm volatile("");
    item_id = 0x1ff;
    item_id &= raw;
    if (Item_Get(item_id)->type == 6)
        return 0;
    if (Item_Get(item_id)->flags & 8)
        return 0;
    Func_0810a108(unit_id, slot, -1);
    return 1;
}
