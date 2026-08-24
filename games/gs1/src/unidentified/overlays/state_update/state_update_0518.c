#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_000000b0
#define SecondaryRuntimeSelector Value_000000af
#define TertiaryRuntimeSelector Value_000000ae
#define PrimaryOverlayData Data_0200b694
#define SecondaryOverlayData Data_0200b754
#define TertiaryOverlayData Data_0200b784
#define DefaultOverlayData Data_0200b88c
#define SelectOverlayDataByRuntimeSelector Func_02000fdc

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
