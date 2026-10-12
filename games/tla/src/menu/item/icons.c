#include "types.h"
#include "ram_buffer.h"
#include "owner_state.h"

struct ItemEntryIcon {
    u8 unknown_00[5];
    u8 state;
    u8 unknown_06[8];
    u8 slot;
};

struct ItemEntryMenu {
    u8 unknown_000[0x24];
    s32 item_window;
    u8 unknown_028[0x24];
    struct ItemEntryIcon *entry_icons[32];
    u8 unknown_0cc[0xf8];
    u16 items[32];
    u8 unknown_204[0x10];
    u8 item_count;
};

void UiIcon_PrepareObject(struct ItemEntryIcon *icon);
void Func_08038288(s32 mode, s32 id, s32 slot, s32 flags);
void ItemMenu_RefreshEntry(s32 mode);

void Menu_HideEmptyEntryIcons(const u16 *items);

s32 ItemMenu_Collect(struct OwnerInventoryState *owner, u16 *items, s32 mode)
{
    s32 count;
    s32 i;

    /* FAKEMATCH: the empty one-pass block before the clearing loop keeps the zero's literal pool before the second loop (differ 10 without it). */
    do { } while (0);
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
    struct ItemEntryIcon **icons;
    s32 item_id;

    icons = ((struct ItemEntryMenu *)Ram_HeapSlots->menu_runtime)->entry_icons;
    entries = items;
    remaining = 14;
    do {
        item_id = *entries++;
        if (item_id != 0) {
            if (style == 0) {
                Func_08038288(2, item_id, (*icons)->slot, 0);
            } else {
                Func_08038288(7, item_id, (*icons)->slot, 0);
            }
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(items);
}

extern char MsgItemMenuEmpty[];

s32 ItemMenu_Count(s32 owner_id);
void RenderOutput_RedrawSavedRectFar(s32 window);
s32 UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);

void ItemMenu_RefreshOwner(s32 owner_id, s32 mode)
{
    struct ItemEntryMenu *menu;
    struct OwnerInventoryState *owner;
    u16 *items;

    menu = Ram_HeapSlots->menu_runtime;
    owner = UnitGet(owner_id);
    items = menu->items;
    menu->item_count = ItemMenu_Collect(owner, items, 0);
    RenderOutput_RedrawSavedRectFar(menu->item_window);
    ItemMenu_RefreshEntry(mode);
    ItemMenu_DrawIcons(items, 0);
    if (ItemMenu_Count(owner_id) == 0)
        UiText_DrawCharacterAtOffsetFar((s32)MsgItemMenuEmpty, menu->item_window, 8, 24);
}
