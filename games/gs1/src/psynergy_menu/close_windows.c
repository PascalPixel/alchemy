#include "psynergy_menu.h"
#include "global_cells.h"

s32 Func_08015018(s32 window, s32 mode);
void Func_080a195c(void);

void PsynergyMenu_CloseWindows(void)
{
    struct PsynergyMenuState *menu;

    menu = *(struct PsynergyMenuState **)ADDR_03001F2C;
    Func_080a195c();
    Func_08015018(menu->auxiliary_window, 1);
    Func_08015018(menu->psynergy_window, 1);
    Func_08015018(menu->message_window, 1);
}
