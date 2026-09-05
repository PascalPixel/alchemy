#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define UiText_GetResourceDimensionsAlt Func_080187fc

extern s32 UiText_BuildRenderEntries(s32, s32);
extern void Func_0801868c(s32, s32, s32, s32, s32, s32, s32);

s32 UiText_GetResourceDimensionsAlt(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u16 *base;
    s32 idx;
    s32 ofs;

    base = *(u16 **)ADDR_03001E8C;
    idx = UiText_BuildRenderEntries(no, 0);
    ofs = idx * 2 + RENDER_ENTRY_TBL_OFS;
    if (*(u16 *)((u8 *)base + ofs) == 0)
    {
        return 0;
    }
    Func_0801868c(idx, arg1, arg2, arg3, arg4, 0, 1);
    return 1;
}
