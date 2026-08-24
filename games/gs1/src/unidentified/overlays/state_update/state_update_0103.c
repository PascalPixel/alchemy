#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000060
#define SecondaryRuntimeSelector Value_00000061
#define TertiaryRuntimeSelector Value_00000062
#define PrimaryOverlayData Data_02009f98
#define SecondaryOverlayData Data_0200a064
#define TertiaryOverlayData Data_0200a190
#define DefaultOverlayData Data_02009f8c
#define SelectOverlayDataByRuntimeSelector Func_020002e0

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
