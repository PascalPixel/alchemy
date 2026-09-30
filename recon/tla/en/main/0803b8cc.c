#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
extern u8 Data_03001e8c[];

s32 UiText_BuildRenderEntries(s32, s32);

s32 UiText_GetResourceDimensionsAlt(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u16 *base;
    s32 idx;
    s32 ofs;

    base = *(u16 **)((u32)&Data_03001e8c);
    idx = UiText_BuildRenderEntries(no, 0);
    ofs = idx * 2 + RENDER_ENTRY_TBL_OFS;
    if (*(u16 *)((u8 *)base + ofs) == 0)
    {
        return 0;
    }
    UiWindow_FitOnScreen(idx, arg1, arg2, arg3, arg4, 0, 1);
    return 1;
}
