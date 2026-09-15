#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

void UiWork_SetRenderWord(u16 value)
{
    *(u16 *)(*(u8 **)ADDR_03001E8C + RENDER_WORD_OFS) = value;
}
