#include "scene.h"
#include "abi/psynergy_menu/create_entry_grid.h"
#include "psynergy_menu.h"
#include "global_cells.h"

s32 InitializeEntryObjects(s32, s32, s32, s32, s32);
s32 UiWindow_CreateFar(s32, s32, s32, s32, s32);
struct PsynergyMenuIcon *Sys_Run(s32, s32, s32);
void *Sys_Run2(s32, s32, s32, s32, s32, s32);
struct PsynergyMenuIcon *Sys_Run3(s32, s32, s32, s32, s32);

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
    window = Sys_Check(menu);
    InitializeEntryObjects(window, 2, 2, 8, 0);

    window = UiWindow_CreateFar(0, 5, 30, 15, 2);
    menu->psynergy_window = window;
    menu->selected_column = 0;
    menu->selected_row = 0;
    menu->column_count = 8;
    menu->row_count = 2;

    cursor = Sys_Run(window, 0, 4);
    cursor->state = 13;
    menu->entry_grid_cursor = cursor;
    Sys_Run2(0, 0, 0, window, 0, 0);

    y = 8;
    index = 0;
    output = &menu->entry_icons[0];
    x = 96;
    do {
        *output++ = Sys_Run3(4, index, window, x, y);
        index++;
        x += 16;
    } while (index <= 7);

    index = 8;
    y = 24;
    output = &menu->entry_icons[8];
    x = 96;
    do {
        *output++ = Sys_Run3(4, index, window, x, y);
        index++;
        x += 16;
    } while (index <= 15);
}
