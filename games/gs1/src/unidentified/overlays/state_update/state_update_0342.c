#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_000000b0
#define SecondaryRuntimeSelector Value_000000af
#define TertiaryRuntimeSelector Value_000000ae
#define PrimaryOverlayData Data_0200b270
#define SecondaryOverlayData Data_0200b330
#define TertiaryOverlayData Data_0200b4f8
#define DefaultOverlayData Data_0200b558
#define SelectOverlayDataByRuntimeSelector Func_02000f58

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
