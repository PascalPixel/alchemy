#include "types.h"

struct MenuPageIcon {
    u8 reserved_00[5];
    u8 state;
    u16 x;
    u16 y;
};

struct MenuPageWork {
    u8 reserved_000[0x4c];
    struct MenuPageIcon *icons[32];
    u8 reserved_0cc[0x148];
    u8 entry_count;
};

#include "ram_buffer.h"

void UiIcon_PrepareObject(struct MenuPageIcon *icon);

void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y)
{
    struct MenuPageWork *menu = Ram_HeapSlots->menu_runtime;
    struct MenuPageIcon *icon;
    s32 i;

    for (i = 0; i < 32; i++) {
        icon = menu->icons[i];
        if (icon != NULL)
            icon->state = 13;
    }
    for (i = first; i < page_size + first && (icon = menu->icons[i]) != NULL
         && i <= menu->entry_count - 1; i++) {
        icon->x = x;
        icon->y = y + (i - first) * 16;
        UiIcon_PrepareObject(icon);
        icon->state = 1;
    }
}
