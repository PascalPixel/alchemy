#include "types.h"
#include "inventory_menu.h"
#include "item.h"

#define COMMAND_DISABLED (-1)
#define COMMAND_AVAILABLE 1

s32 Func_080a46b4(s32 owner, s32 item);
s32 Func_0808a490(s32 item);

void InventoryMenu_BuildCommandStates(s8 *command_states)
{
    struct InventoryMenuState *menu;
    struct ItemDefinition *item;

    menu = Data_03001f2c;
    item = Item_Get(0x1ff & menu->selected_item);

    if (item->type == 0) {
        command_states[0] = COMMAND_AVAILABLE;
        command_states[1] = COMMAND_DISABLED;
    } else {
        command_states[0] = COMMAND_DISABLED;
        command_states[1] = COMMAND_AVAILABLE;
    }

    if (Func_080a46b4(menu->item_owner, menu->selected_item) != -1)
        command_states[0] = COMMAND_AVAILABLE;
    else
        command_states[0] = COMMAND_DISABLED;

    if (menu->selected_item & 0x400)
        command_states[0] = COMMAND_DISABLED;

    if (Item_CanOwnerEquip(
            menu->item_owner,
            menu->selected_item & 0x1ff) == 0) {
        command_states[1] = COMMAND_DISABLED;
    }

    command_states[3] = COMMAND_AVAILABLE;
    command_states[5] = COMMAND_AVAILABLE;
    command_states[2] = COMMAND_AVAILABLE;

    if (menu->selected_item & 0x200) {
        command_states[4] = COMMAND_AVAILABLE;
        command_states[1] = COMMAND_DISABLED;
    } else {
        command_states[4] = COMMAND_DISABLED;
    }

    if (item->flags & 2) {
        command_states[4] = COMMAND_DISABLED;
        if (menu->selected_item & 0x200) {
            command_states[3] = COMMAND_DISABLED;
            command_states[5] = COMMAND_DISABLED;
        }
    }

    if (Func_0808a490(menu->selected_item & 0x1ff) != 0)
        command_states[0] = COMMAND_AVAILABLE;

    if (menu->party_count <= 1)
        command_states[3] = COMMAND_DISABLED;

    if (item->flags & 8)
        command_states[5] = COMMAND_DISABLED;
}
