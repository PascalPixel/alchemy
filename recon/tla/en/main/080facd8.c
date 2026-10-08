#include "far_runtime.h"
#include "owner_state.h"

void Menu_HideEmptyEntryIcons(const u16 *items)
{
    struct InventoryMenuState *menu = gMenuWork;
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            UiIcon_PrepareObject(menu->entry_icons[slot]);
            menu->entry_icons[slot]->state = 13;
        }
    }
}
