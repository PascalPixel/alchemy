#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_000000b0
#define SecondaryRuntimeSelector Value_000000af
#define TertiaryRuntimeSelector Value_000000ae
#define PrimaryOverlayData Data_0200b91c
#define SecondaryOverlayData Data_0200b9e8
#define TertiaryOverlayData Data_0200bac0
#define DefaultOverlayData Data_0200bc28

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectTableB91cByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
