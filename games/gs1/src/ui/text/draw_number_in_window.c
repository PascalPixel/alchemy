#include "types.h"

u8 *Text_FormatNumber(u8 *, s32, s32);
void UiText_DrawStringInWindow(u8 *s, s32 arg1, u32 arg2, u32 arg3);

void UiText_DrawNumberInWindow(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawStringInWindow(Text_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}
