#include "types.h"
#include "gs1_edition.h"

extern u8 * volatile Data_03001e8c;

void UiWork_SetMenuBusy(void)
{
    u8 *base = Data_03001e8c;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 1;
    *p = flag;
}

void Func_080a2420(void)
{
    u8 *base = Data_03001e8c;
    u8 *p = base + RENDER_MENU_BUSY_OFS;
    u8 flag = 0;
    *p = flag;
}
