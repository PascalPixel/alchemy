#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000001e
#define SecondaryRuntimeSelector Value_00000023
#define TertiaryRuntimeSelector Value_00000020
#define PrimaryOverlayData Data_02009498
#define SecondaryOverlayData Data_02009600
#define TertiaryOverlayData Data_020096f0
#define DefaultOverlayData Data_02009480
#define SelectOverlayDataByRuntimeSelector Func_0200011c

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
