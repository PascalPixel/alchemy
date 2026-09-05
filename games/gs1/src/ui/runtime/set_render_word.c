#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#define UiWork_SetRenderWord Func_0801e738

void UiWork_SetRenderWord(u16 value)
{
    *(u16 *)(*(u8 **)ADDR_03001E8C + RENDER_WORD_OFS) = value;
}
