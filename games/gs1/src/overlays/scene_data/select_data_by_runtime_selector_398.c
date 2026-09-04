#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000031
#define SecondaryRuntimeSelector Value_00000030
#define TertiaryRuntimeSelector Value_0000002f
#define PrimaryOverlayData Data_02008ea8
#define SecondaryOverlayData Data_02008efc
#define TertiaryOverlayData Data_02008f80
#define DefaultOverlayData Data_02008e9c

#include "select_overlay_data_by_runtime_selector.h"

#define SceneData_SelectDataByRuntimeSelector Func_020000f4

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
