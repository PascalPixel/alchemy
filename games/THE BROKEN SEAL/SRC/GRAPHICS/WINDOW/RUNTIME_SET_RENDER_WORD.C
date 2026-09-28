#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"
extern u8 Data_03001e8c[];

void UiWork_SetRenderWord(u16 value)
{
    *(u16 *)(*(u8 **)((u32)&Data_03001e8c) + RENDER_WORD_OFS) = value;
}
