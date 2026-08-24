#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000060
#define SecondaryRuntimeSelector Value_00000061
#define TertiaryRuntimeSelector Value_00000062
#define PrimaryOverlayData Data_020098cc
#define SecondaryOverlayData Data_02009a34
#define TertiaryOverlayData Data_02009b9c
#define DefaultOverlayData Data_0200989c
#define SelectOverlayDataByRuntimeSelector Func_02000030

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
