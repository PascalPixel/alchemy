#include "mem.h"
#include "inventory_menu.h"
#include "btlsys.h"
#include "types.h"
#include "item.h"
#include "iwram_call.h"
#include "ui.h"
#include "character_menu.h"
#include "menu_list.h"
#include "inventory.h"
#include "game.h"
struct ItemDefinition *Item_GetDirectFar(s32 item);

s32 Inventory_RemoveFirstUnflagged(s32 owner);

s32 ItemMenu_Collect(struct BtlUnit *owner, u16 *items, s32 mode)
{
    s32 count;
    s32 i;

    for (i = 0; i < 32; i++) {
        items[i] = 0;
    }
    count = 0;
    for (i = 0; i < 15; i++) {
        items[i] = 0;
        if (owner->inventory[i] != 0) {
            items[count] = owner->inventory[i];
            count++;
        }
    }
    return count;
}

void ItemMenu_DrawIcons(u16 *items, s32 style)
{
    s32 remaining;
    u16 *entries;
    struct RenderOutput **icons;
    s32 item_id;

    icons = gMenuWork->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                Resource_LoadByModeIntoSlotFar(
                    2, item_id, (u8)(*icons)->index, 0);
            } else {
                Resource_LoadByModeIntoSlotFar(
                    7, item_id, (u8)(*icons)->index, 0);
            }
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(items);
}

void ItemMenu_RefreshOwner(s32 owner_id, s32 mode)
{
    struct InventoryMenuState *menu;
    struct BtlUnit *owner;
    u16 *items;

    menu = gMenuWork;
    owner = UnitGetFar(owner_id);
    items = menu->items;
    menu->item_count = ItemMenu_Collect(owner, items, 0);
    RenderOutput_RedrawSavedRectFar(menu->item_window);
    ItemMenu_RefreshEntry(mode);
    ItemMenu_DrawIcons(items, 0);
    if (ItemMenu_Count(owner_id) == 0)
        UiText_DrawCharacterAtOffsetFar(
            (s32)MsgItemMenuEmpty, (s32)menu->item_window, 8, 24);
}

void InventoryMenu_NoOp(void)
{
}

void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target)
{
    struct InventoryMenuState *menu = gMenuWork;
    register s32 style asm("sl") = 0; /* FAKEMATCH: the ROM allocates owner (r8) before style (sl); style's 19 references over 246 insns outrank owner's 4 over 28 */
    struct BtlUnit *state = UnitGetFar(owner);
    s32 item = state->inventory[slot];
    struct BtlUnit *saved;
    s32 equipped;

    if (mode == 1)
        style = 0x100;
    switch (Item_GetDirectFar(item & ITEM_ID_MASK)->type) {
    case 0:
        Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
        if (owner == target) {
            style |= 2;
            Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
        } else {
            s32 size;

            state = UnitGetFar(target);
            size = BATTLE_UNIT_SIZE;
            saved = MemAlloc(size);
            Iwram_CopyWords(saved, state, size);
            equipped = Inventory_RemoveFirstUnflagged(target);
            if (equipped != 0) {
                item &= ~0x200;
                equipped = Inventory_AddItemFar(target, item);
                if (equipped != -1) {
                    style |= 2;
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, equipped, style);
                } else {
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
                }
            } else {
                Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
            }
            Iwram_CopyWords(state, saved, size);
            MemFree(saved);
        }
        break;
    case 6:
        if (target == owner) {
            style |= 4;
            Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
        } else {
            s32 size;

            state = UnitGetFar(target);
            size = BATTLE_UNIT_SIZE;
            saved = MemAlloc(size);
            Iwram_CopyWords(saved, state, size);
            equipped = Inventory_RemoveFirstUnflagged(target);
            if (equipped != 0) {
                equipped = Inventory_AddItemFar(target, item);
                if (equipped != -1) {
                    style |= 4;
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, equipped, style);
                } else {
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
                }
            } else {
                Menu_DrawOwnerStatusPanel((s32)menu->status_window, target, slot, style);
            }
            Iwram_CopyWords(state, saved, size);
            MemFree(saved);
        }
        break;
    }
}

s32 Inventory_RemoveFirstUnflagged(s32 owner)
{
    struct BtlUnit *state = UnitGetFar(owner);
    s32 result = 0;
    s32 i;

    for (i = 0; i < 15; i++) {
        if (state->inventory[i] == 0) {
            result = 1;
            break;
        }
        if ((state->inventory[i] & INVENTORY_EQUIPPED) == 0) {
            s32 quantity = state->inventory[i] >> INVENTORY_QUANTITY_SHIFT;
            s32 count = quantity + 1;
            s32 cnt;

            if (quantity == 0)
                count = 1;
            for (cnt = 0; cnt < count; cnt++)
                result = Inventory_RemoveFar(owner, i);
            if (result != 2)
                return 0;
            result = 1;
            break;
        }
    }
    return result;
}
