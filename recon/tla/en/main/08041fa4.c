#include "TEXT_RENDER_RUNTIME.H"
#include "RUNTIME_MEM.H"
extern u8 Data_03001e8c[];

extern u8 *gWindowWork;

s32 UiText_BuildRenderEntries(s32 character, s32 count);
s16 *Runtime_BumpAllocateAlternatePool(s32 size);
u8 *UiText_FormatNumber(u8 *output, s32 value, s32 width);
void UiText_RenderWideStringAtOffset(void *text, s32 work, s32 x, s32 y);
void UiText_RenderWideStringInWindow(u16 *text, s32 work, s32 x, s32 y);
s32 UiText_RenderStringTiles(void *text, s32 source, s32 destination, s32 phase);

void UiText_DrawResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *base = *(u8 **)((u32)&Data_03001e8c);
    u16 *counter = (u16 *)(base + RENDER_ENTRY_COUNT_OFS);
    s32 offset;
    s32 zero = 0;

    *counter = zero;
    UiText_BuildRenderEntries(arg0, 1);
    offset = *counter * 2 + RENDER_ENTRY_TBL_OFS;
    *(u16 *)(base + offset) = zero;
    *counter = (*counter + 1) & RENDER_ENTRY_MASK;
    /* 0xeb0から始まる列を次の処理へ渡す。 */
    UiText_RenderWideStringAtOffset(base + RENDER_ENTRY_TBL_OFS, arg1, arg2, arg3);
}
