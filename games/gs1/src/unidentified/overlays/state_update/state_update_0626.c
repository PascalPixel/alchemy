#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000036
#define SecondaryRuntimeSelector Value_00000037
#define TertiaryRuntimeSelector Value_00000038
#define PrimaryOverlayData Data_0200ead8
#define SecondaryOverlayData Data_0200ec10
#define TertiaryOverlayData Data_0200ed60
#define DefaultOverlayData Data_0200eec8
#define SelectOverlayDataByRuntimeSelector Func_02000f30

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
