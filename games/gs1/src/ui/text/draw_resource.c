#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

void UiText_BuildRenderEntries(s32, s32);
void Func_08017aa4(u8 *, s32, s32, s32);

void UiText_DrawResource(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *base = *(u8 **)ADDR_03001E8C;
    u16 *counter = (u16 *)(base + RENDER_ENTRY_COUNT_OFS);
    s32 offset;
    s32 zero = 0;

    *counter = zero;
    UiText_BuildRenderEntries(arg0, 1);
    offset = *counter * 2 + RENDER_ENTRY_TBL_OFS;
    *(u16 *)(base + offset) = zero;
    *counter = (*counter + 1) & RENDER_ENTRY_MASK;
    /* 0xeb0から始まる列を次の処理へ渡す。 */
    Func_08017aa4(base + RENDER_ENTRY_TBL_OFS, arg1, arg2, arg3);
}
