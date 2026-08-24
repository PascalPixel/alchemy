#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000001e
#define SecondaryRuntimeSelector Value_00000023
#define TertiaryRuntimeSelector Value_00000020
#define PrimaryOverlayData Data_02009744
#define SecondaryOverlayData Data_02009a2c
#define TertiaryOverlayData Data_02009bc4
#define DefaultOverlayData Data_02009738
#define SelectOverlayDataByRuntimeSelector Func_02000198

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
