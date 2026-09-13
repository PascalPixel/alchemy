#include "far_runtime.h"
#include "inventory_menu.h"
#include "owner_state.h"


s32 ItemMenu_IsSpecial(s32 item_id)
{
    s32 result;
    s32 first;

    if (item_id > 0xC4) {
        goto L0;
    }
    first = 0xC1;
    if (item_id < first) {
        goto L0;
    }
    result = 1;
    return result;
L0:
    result = 0;
    return result;
}


s32 UiText_DrawAt(s32, s32, s32, s32);

void ItemMenu_DrawMsg(s32 unused, s32 message)
{
    struct InventoryMenuState *menu;

    menu = Data_03001f2c;
    UiWindow_Commit(menu->message_window);
    UiText_DrawAt(message, menu->message_window, 0, 0);
}


void Func_080a17c4(void *icon);

void Menu_HideEmptyEntryIcons(const u16 *items)
{
    struct InventoryMenuState *menu = Data_03001f2c;
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            Func_080a17c4(menu->entry_icons[slot]);
            menu->entry_icons[slot]->state = 13;
        }
    }
}


s32 ItemMenu_Count(s32 owner_id)
{
    s32 item_id;
    s32 remaining;
    s32 count;
    u16 *slots;

    count = 0;
    slots = OwnerState_GetFar(owner_id)->inventory;
    remaining = 0xE;
    do {
        item_id = 0x1FF & *slots;
        slots += 1;
        if (item_id != 0) {
            count += 1;
        }
        remaining -= 1;
    } while (remaining >= 0);
    return count;
}
