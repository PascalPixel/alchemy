#include "text_render_runtime.h"

u8 *UiText_FormatNumber(u8 *output, s32 value, s32 width);

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    UiText_DrawString(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}
