#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000060
#define SecondaryRuntimeSelector Value_00000061
#define TertiaryRuntimeSelector Value_00000062
#define PrimaryOverlayData Data_02009c80
#define SecondaryOverlayData Data_02009cc0
#define TertiaryOverlayData Data_02009cfc
#define DefaultOverlayData Data_02009c7c
#define SelectOverlayDataByRuntimeSelector Func_0200008c

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
