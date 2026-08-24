#include "types.h"

s32 UiWindow_Create(s32, s32, s32, s32, s32);
void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);
extern u8 Data_08037300[];
s32 Func_08021c34(void)
{
    s32 handle = UiWindow_Create(0, 0, 6, 4, 6);

    UiText_DrawStringInWindow(Data_08037300, handle, 0, 0);
    return handle;
}
