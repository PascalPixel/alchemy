#include "inventory_menu.h"
#include "global_cells.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_JA)
#define PAGE_X  120
#define ENTRY_X 32
#else
#define PAGE_X  116
#define ENTRY_X 24
#endif

extern u8 Value_00000182;

void Func_08015070(s32 window, s32 x, s32 width, s32 height, s32 style);
void Func_08015080(s32 message, s32 window, s32 x, s32 y);
void Func_08015270(s32 window);
void Func_080a21b0(s32 window, s32 count, s32 page_size, s32 page, s32 style);
void Func_080a2324(s32 page_size, s32 first_entry, s32 window, s32 x, s32 y);

s32 InventoryMenu_DrawItemNamePage(
    s32 window,
    s32 unused,
    const struct MenuResult *state)
{
    struct InventoryMenuState *menu =
        *(struct InventoryMenuState **)ADDR_03001F2C;
    u32 page;
    u32 first_entry;
    u32 visible_count;
    u8 row;
    const u16 *item_id;

    (void)unused;

    Func_08015270(window);
    Func_08015070(window, 0, 11, 16, 11);

    page = state->page;
    first_entry = page * 5;
    visible_count = (u8)(state->entry_count - first_entry);
    if (visible_count > 5) {
        visible_count = 5;
    }

    Func_080a2324(5, first_entry, window, PAGE_X, 34);
    Func_080a21b0(window, state->entry_count, 5, state->page, 15);

    row = 0;
    if (visible_count > row) {
        item_id = &menu->items[first_entry];
        do {
            Func_08015080(
                (item_id[0] & 0x1ff) + (s32)&Value_00000182,
                menu->item_window,
                ENTRY_X,
                row * 16 + 8
            );
            row++;
            item_id++;
        } while (visible_count > row);
    }

    return 1;
}

s32 InventoryMenu_ItemNamePageReturnTrue(void)
{
    return 1;
}
