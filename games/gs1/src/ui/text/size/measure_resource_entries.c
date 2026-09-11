#include "types.h"
#include "scene.h"
#include "abi/ui/text/size/measure_resource_entries.h"
#include "runtime_interfaces.h"

s32 UiText_BuildRenderEntries(s32, s32);

void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y)
{
    Ui_Run(UiText_BuildRenderEntries(no, 0), x, y, 0);
}
