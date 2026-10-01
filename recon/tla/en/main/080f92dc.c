/* Near miss: score 60. The menu page work is ⚓️'s heap slot menu_page_work,
   its icons 4 bytes later and its entry count at 0x214. ⚓️ reloads the
   stacked y argument (ldr r7, [sp, #32]) earlier; a load-scheduling
   difference that -mtune=arm9tdmi narrows but does not close. */
#include "TYPES.H"

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

#include "RAM_BUFFER.H"

void UiIcon_PrepareObject(struct MenuPageIcon *icon);

/*
 * Hides every entry icon, then shows one page of them: up to page_size
 * icons from the first entry, placed in a column at (x, y), 16 pixels
 * apart, stopping at a missing icon or the end of the list.
 */

void Menu_SetPageIcons(s32 page_size, s32 first, s32 window, s32 x, s32 y)
{
    struct MenuPageWork *menu = Ram_HeapSlots->menu_page_work;
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
