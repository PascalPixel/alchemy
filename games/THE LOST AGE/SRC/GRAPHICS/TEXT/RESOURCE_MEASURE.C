#include "TYPES.H"

s32 UiText_BuildRenderEntries(s32, s32);

void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y)
{
    UiText_MeasureEntryDimensions(UiText_BuildRenderEntries(no, 0), x, y, 0);
}
