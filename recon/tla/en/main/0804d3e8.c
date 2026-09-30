#include "TYPES.H"

struct CenterEntry {
    u8 unknown_00[12];
    s16 x;
    s16 y;
    u8 unknown_10[4];
};

struct CenterMenu {
    struct CenterEntry entries[6];
    s32 window;                     /* 0x78 */
    u8 unknown_7c[0x8e - 0x7c];
    s16 count;                      /* 0x8e */
    s16 width;
    s16 height;
    s16 row;
};

extern struct CenterMenu *gMenuSelectWork;

s32 UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 style);

/* Lays the menu's entries out three tiles apart on the given tile row,
   centred with the window for their text, and opens that window. */

void Menu_CenterResourceEntries(s32 row, s32 width, s32 height)
{
    struct CenterMenu *menu = gMenuSelectWork;
    s32 x;
    s32 i;
    s32 count;

    menu->width = width + 2;
    menu->height = height;
    menu->row = row;
    count = menu->count;
    x = 15 - (count * 3 + menu->width * 2 / 3) / 2;
    for (i = 0; i < menu->count; i++) {
        struct CenterEntry *entry = &menu->entries[i];

        entry->x = x * 8;
        entry->y = row * 8;
        x += 3;
    }
    menu->window = UiWindow_Create(x, row, menu->width, 3, 2);
}
