#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000006b
#define SecondaryRuntimeSelector Value_00000070
#define TertiaryRuntimeSelector Value_0000006c
#define PrimaryOverlayData Data_02009738
#define SecondaryOverlayData Data_020097c8
#define TertiaryOverlayData Data_02009840
#define DefaultOverlayData Data_02009708
#define SelectOverlayDataByRuntimeSelector Func_02000040

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
