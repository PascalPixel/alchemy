#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000044
#define SecondaryRuntimeSelector Value_00000045
#define TertiaryRuntimeSelector Value_00000046
#define PrimaryOverlayData Data_0200ba48
#define SecondaryOverlayData Data_0200bb20
#define TertiaryOverlayData Data_0200bc1c
#define DefaultOverlayData Data_0200bd54

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectTableBa48ByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
