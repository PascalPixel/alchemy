#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_000000a4
#define SecondaryRuntimeSelector Value_000000a5
#define TertiaryRuntimeSelector Value_000000a6
#define PrimaryOverlayData Data_02009488
#define SecondaryOverlayData Data_020094d0
#define TertiaryOverlayData Data_02009548
#define DefaultOverlayData Data_02009458
#define SelectOverlayDataByRuntimeSelector Func_0200034c

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
