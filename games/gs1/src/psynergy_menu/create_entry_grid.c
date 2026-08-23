#include "psynergy_menu.h"
#include "global_cells.h"

s32 Func_080a1814(void *menu);
s32 Func_080a1870(s32, s32, s32, s32, s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
struct PsynergyMenuIcon *Func_080a1778(s32, s32, s32);
void *Func_080150d8(s32, s32, s32, s32, s32, s32);
struct PsynergyMenuIcon *Func_080150d0(s32, s32, s32, s32, s32);

void PsynergyMenu_CreateEntryGrid(void)
{
    s32 window;
    struct PsynergyMenuState *menu;
    struct PsynergyMenuIcon *cursor;
    s32 index;
    s32 x;
    s32 y;
    struct PsynergyMenuIcon **output;

    menu = *(struct PsynergyMenuState **)ADDR_03001F2C;
    window = Func_080a1814(menu);
    Func_080a1870(window, 2, 2, 8, 0);

    window = Func_08015010(0, 5, 30, 15, 2);
    menu->psynergy_window = window;
    menu->selected_column = 0;
    menu->selected_row = 0;
    menu->column_count = 8;
    menu->row_count = 2;

    cursor = Func_080a1778(window, 0, 4);
    cursor->state = 13;
    menu->entry_grid_cursor = cursor;
    Func_080150d8(0, 0, 0, window, 0, 0);

    y = 8;
    index = 0;
    output = &menu->entry_icons[0];
    x = 96;
    do {
        *output++ = Func_080150d0(4, index, window, x, y);
        index++;
        x += 16;
    } while (index <= 7);

    index = 8;
    y = 24;
    output = &menu->entry_icons[8];
    x = 96;
    do {
        *output++ = Func_080150d0(4, index, window, x, y);
        index++;
        x += 16;
    } while (index <= 15);
}
