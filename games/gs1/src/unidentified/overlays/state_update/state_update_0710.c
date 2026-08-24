#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_000000a4
#define SecondaryRuntimeSelector Value_000000a5
#define TertiaryRuntimeSelector Value_000000a6
#define PrimaryOverlayData Data_02009610
#define SecondaryOverlayData Data_020096b8
#define TertiaryOverlayData Data_02009790
#define DefaultOverlayData Data_020095f8
#define SelectOverlayDataByRuntimeSelector Func_020003ac

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
