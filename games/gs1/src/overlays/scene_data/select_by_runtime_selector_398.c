#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000031
#define SecondaryRuntimeSelector Value_00000030
#define TertiaryRuntimeSelector Value_0000002f
#define PrimaryOverlayData Data_020089ec
#define SecondaryOverlayData Data_02008a64
#define TertiaryOverlayData Data_02008b24
#define DefaultOverlayData Data_020089bc
#define SceneData_SelectByRuntimeSelector Func_02000040

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
