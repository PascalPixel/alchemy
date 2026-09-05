#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000044
#define SecondaryRuntimeSelector Value_00000045
#define TertiaryRuntimeSelector Value_00000046
#define PrimaryOverlayData Data_0200b0f4
#define SecondaryOverlayData Data_0200b1e4
#define TertiaryOverlayData Data_0200b334
#define DefaultOverlayData Data_0200b4b4

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
