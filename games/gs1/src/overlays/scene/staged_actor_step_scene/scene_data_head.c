#include "types.h"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000075
#define SecondaryRuntimeSelector Value_00000076
#define TertiaryRuntimeSelector Value_00000078
#define PrimaryOverlayData Data_0200a898
#define SecondaryOverlayData Data_0200a8e0
#define TertiaryOverlayData Data_0200a928
#define DefaultOverlayData Data_0200a868
#define SceneData_SelectTableA898ByRuntimeSelector Func_020009f0
#define SceneData_ReturnZero Func_02000a44
#define SceneData_GetTablea970 Func_02000a48

#include "select_overlay_data_by_runtime_selector.h"

/* Contiguous unnamed leaf-owner run for resource_3b4. */

s32 SceneData_SelectTableA898ByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTablea970(void)
{
    return (u8 *)0x0200a970;
}
