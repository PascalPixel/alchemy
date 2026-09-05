#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000093
#define SecondaryRuntimeSelector Value_00000095
#define TertiaryRuntimeSelector Value_00000097
#define PrimaryOverlayData Data_0200c1b0
#define SecondaryOverlayData Data_0200c270
#define TertiaryOverlayData Data_0200c318
#define DefaultOverlayData Data_0200c198

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
