#include "TYPES.H"

struct MenuPageIcon {
    u8 reserved_00[5];
    u8 state;
    u16 x;
    u16 y;
};

struct MenuPageWork {
    u8 reserved_000[0x48];
    struct MenuPageIcon *icons[32];
    u8 reserved_0c8[0x150];
    u8 entry_count;
};

extern struct MenuPageWork *gMenuWork;

void UiIcon_PrepareObject(struct MenuPageIcon *icon);

/*
 * Hides every entry icon, then shows one page of them: up to page_size
 * icons from the first entry, placed in a column at (x, y), 16 pixels
 * apart, stopping at a missing icon or the end of the list.
 */

void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y)
{
    struct MenuPageWork *menu = gMenuWork;
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
