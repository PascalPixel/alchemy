#include "psynergy_menu.h"
#include "global_cells.h"

s32 UiWindow_Close(s32 window, s32 mode);
void Menu_ReleaseEntryObjects(void);

void PsynergyMenu_CloseWindows(void)
{
    struct PsynergyMenuState *menu;

    menu = *(struct PsynergyMenuState **)ADDR_03001F2C;
    Menu_ReleaseEntryObjects();
    UiWindow_Close(menu->auxiliary_window, 1);
    UiWindow_Close(menu->psynergy_window, 1);
    UiWindow_Close(menu->message_window, 1);
}
