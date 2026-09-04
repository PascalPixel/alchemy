#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000005d
#define SecondaryRuntimeSelector Value_0000005e
#define TertiaryRuntimeSelector Value_0000005f
#define PrimaryOverlayData Data_02009f9c
#define SecondaryOverlayData Data_0200a014
#define TertiaryOverlayData Data_0200a134
#define DefaultOverlayData Data_02009f6c
#define SceneData_SelectByRuntimeSelector Func_02000d20

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
