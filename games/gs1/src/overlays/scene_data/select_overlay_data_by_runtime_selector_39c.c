#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000039
#define SecondaryRuntimeSelector Value_00000038
#define TertiaryRuntimeSelector Value_00000037
#define PrimaryOverlayData Data_0200e50c
#define SecondaryOverlayData Data_0200e35c
#define TertiaryOverlayData Data_0200e23c
#define DefaultOverlayData Data_0200e0ec

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
