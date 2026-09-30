#include "TYPES.H"
#include "RUNTIME_INTERFACES.H"
extern u8 Data_03001e8c[];

s32 UiText_BuildRenderEntries(s32, s32);

s32 UiText_GetResourceDimensions(s32 no, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u16 *base;
    s32 temp;
    s32 offset;

    base = *(u16 **)((u32)&Data_03001e8c);
    temp = UiText_BuildRenderEntries(no, 0);
    offset = temp * 2 + RENDER_ENTRY_TBL_OFS;
    if (*(u16 *)((u8 *)base + offset) == 0)
    {
        return 0;
    }
    UiWindow_FitOnScreen(temp, arg1, arg2, arg3, arg4, 0, 0);
    return 1;
}
