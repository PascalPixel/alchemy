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

void Menu_HideEmptyEntryIcons(const u16 *items)
{
    struct ItemEntryMenu *menu = Ram_HeapSlots->menu_runtime;
    s32 slot;

    for (slot = 0; slot < 32; slot++) {
        if (items[slot] == 0) {
            UiIcon_PrepareObject(menu->entry_icons[slot]);
            menu->entry_icons[slot]->state = 13;
        }
    }
}
