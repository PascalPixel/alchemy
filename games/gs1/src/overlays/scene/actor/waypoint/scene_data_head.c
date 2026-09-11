#include "types.h"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000044
#define SecondaryRuntimeSelector Value_00000045
#define TertiaryRuntimeSelector Value_00000046
#define PrimaryOverlayData Data_0200b0f4
#define SecondaryOverlayData Data_0200b1e4
#define TertiaryOverlayData Data_0200b334
#define DefaultOverlayData Data_0200b4b4
#define SceneData_SelectByRuntimeSelector Func_02000ee0
#define SceneData_ReturnZero Func_02000f34
#define SceneData_GetTableB5bc Func_02000f38

#include "select_overlay_data_by_runtime_selector.h"

s32 SceneData_SelectByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableB5bc(void)
{
    return (u8 *)0x0200b5bc;
}
