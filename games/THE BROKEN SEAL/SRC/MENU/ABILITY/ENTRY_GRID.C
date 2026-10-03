#include "PSYNERGY_MENU.H"
#include "GLOBAL_CELLS.H"
#include "UI.H"

s32 UiMenu_CreateCursor(void *menu);
/* The four-word portrait initializer retains this caller's extra word. */
void PsynergyMenu_InitializeEntryObjects();
struct RenderOutput *UiIcon_CreateWithResourceVariant(s32, s32, s32);
void *SideObject_CreateFar(s32, s32, s32, s32, s32, s32);
struct RenderOutput *RenderOutput_CreateFromResourceFar(s32, s32, s32, s32, s32);

void PsynergyMenu_CreateEntryGrid(void)
{
    s32 window;
    struct PsynergyMenuState *menu;
    struct RenderOutput *cursor;
    s32 index;
    s32 x;
    s32 y;
    struct RenderOutput **output;

    menu = gMenuWork;
    window = UiMenu_CreateCursor(menu);
    PsynergyMenu_InitializeEntryObjects(window, 2, 2, 8, 0);

    window = UiWindow_CreateFar(0, 5, 30, 15, 2);
    menu->psynergy_window = window;
    menu->selected_column = 0;
    menu->selected_row = 0;
    menu->column_count = 8;
    menu->row_count = 2;

    cursor = UiIcon_CreateWithResourceVariant(window, 0, 4);
    cursor->active = 13;
    menu->entry_grid_cursor = cursor;
    SideObject_CreateFar(0, 0, 0, window, 0, 0);

    y = 8;
    index = 0;
    output = &menu->entry_icons[0];
    x = 96;
    do {
        *output++ = RenderOutput_CreateFromResourceFar(4, index, window, x, y);
        index++;
        x += 16;
    } while (index <= 7);

    index = 8;
    y = 24;
    output = &menu->entry_icons[8];
    x = 96;
    do {
        *output++ = RenderOutput_CreateFromResourceFar(4, index, window, x, y);
        index++;
        x += 16;
    } while (index <= 15);
}

s32 UiWork_FinalizeFar(s32 window, s32 mode);
void Menu_ReleaseEntryObjects(void);

void PsynergyMenu_CloseWindows(void)
{
    struct PsynergyMenuState *menu;

    menu = gMenuWork;
    Menu_ReleaseEntryObjects();
    UiWork_FinalizeFar(menu->auxiliary_window, 1);
    UiWork_FinalizeFar(menu->psynergy_window, 1);
    UiWork_FinalizeFar(menu->message_window, 1);
}

void Resource_LoadByModeIntoSlotFar(s32 style, u16 action, u8 target, s32 flags);

void PsynergyMenu_DrawPsynergyIcons(u16 *psynergies)
{
    s32 remaining;
    struct RenderOutput **icons;
    u16 *p;
    s32 psynergy_id;

    icons =
        gMenuWork->entry_icons;
    p = psynergies;
    remaining = 31;
    do {
        psynergy_id = *p++;
        if (psynergy_id != 0) {
            Resource_LoadByModeIntoSlotFar(
                4, psynergy_id, (u8)(*icons)->index, 0);
        }
        icons++;
        remaining--;
    } while (remaining >= 0);
    Menu_HideEmptyEntryIcons(psynergies);
}
