#include "TEXT_RENDER_RUNTIME.H"
#include "RUNTIME_MEM.H"
extern u8 Data_03001e8c[];

extern u8 *gWindowWork;

s32 UiText_BuildRenderEntries(s32 character, s32 count);
u8 *UiText_FormatNumber(u8 *output, s32 value, s32 width);
void UiText_RenderWideStringAtOffset(void *text, s32 work, s32 x, s32 y);
void UiText_RenderWideStringInWindow(u16 *text, s32 work, s32 x, s32 y);
s32 UiText_RenderStringTiles(void *text, s32 source, s32 destination, s32 phase);

void UiText_DrawNumber(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u8 data[16];

    /* 16バイト一時領域を介して次の処理へ渡す。 */
    UiText_DrawString(UiText_FormatNumber(data, arg0, arg1), arg2, arg3, arg4);
}
