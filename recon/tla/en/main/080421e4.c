/*
 * Draft: UiText_DrawNumber does not yet match; 3 halfwords differ from ☀️'s C, first at +0x8 (ldr r7, [sp, #36]).
 * Links as recon/tla/raw/080421e4.s.
 */
#include "TEXT_RENDER_RUNTIME.H"

u8 *UiText_FormatNumber(u8 *output, s32 value, s32 width);

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawString(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}
