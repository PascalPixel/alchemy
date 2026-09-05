#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000075
#define SecondaryRuntimeSelector Value_00000076
#define TertiaryRuntimeSelector Value_00000078
#define PrimaryOverlayData Data_0200a9b0
#define SecondaryOverlayData Data_0200aa40
#define TertiaryOverlayData Data_0200aad0
#define DefaultOverlayData Data_0200a998

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
