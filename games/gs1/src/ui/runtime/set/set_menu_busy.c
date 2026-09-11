#include "types.h"
#include "scene.h"
#include "abi/ui/runtime/set/set_menu_busy.h"
#include "gs1_edition.h"

extern u8 *volatile gIw;

void UiWork_SetMenuBusy(void)
{
    u8 *base = gIw;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 1;
    *p = flag;
}

void Ui_Run(void)
{
    u8 *base = gIw;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 0;
    *p = flag;
}
