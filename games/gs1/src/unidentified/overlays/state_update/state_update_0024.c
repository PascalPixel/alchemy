#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000060
#define SecondaryRuntimeSelector Value_00000061
#define TertiaryRuntimeSelector Value_00000062
#define PrimaryOverlayData Data_02009d34
#define SecondaryOverlayData Data_02009d4c
#define TertiaryOverlayData Data_02009ecc
#define DefaultOverlayData Data_02009d1c
#define SelectOverlayDataByRuntimeSelector Func_020000e0

#include "select_overlay_data_by_runtime_selector.h"

s32 SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
