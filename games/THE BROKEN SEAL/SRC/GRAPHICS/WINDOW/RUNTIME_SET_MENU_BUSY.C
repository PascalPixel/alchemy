#include "TYPES.H"
#include "TBS_EDITION.H"

extern u8 *volatile gWindowWork;

void UiWork_SetMenuBusy(void)
{
    u8 *base = gWindowWork;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 1;
    *p = flag;
}

void Func_080a2420(void)
{
    u8 *base = gWindowWork;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 0;
    *p = flag;
}
