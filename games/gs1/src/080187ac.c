#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

extern s32 UiText_BuildRenderEntries(s32, s32);
extern void Func_0801868c(s32, s32, s32, s32, s32, s32, s32);

s32 Func_080187ac(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u16 *base;
    s32 temp;
    s32 offset;

    base = *(u16 **)ADDR_03001E8C;
    temp = UiText_BuildRenderEntries(no, 0);
    offset = temp * 2 + RENDER_ENTRY_TBL_OFS;
    if (*(u16 *)((u8 *)base + offset) == 0)
    {
        return 0;
    }
    Func_0801868c(temp, arg1, arg2, arg3, arg4, 0, 0);
    return 1;
}
