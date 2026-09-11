#include "types.h"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000093
#define SecondaryRuntimeSelector Value_00000095
#define TertiaryRuntimeSelector Value_00000097
#define PrimaryOverlayData Data_0200c1b0
#define SecondaryOverlayData Data_0200c270
#define TertiaryOverlayData Data_0200c318
#define DefaultOverlayData Data_0200c198
#define SceneData_GetTablebf70 Func_020009ec
#define SceneData_ReturnZero Func_020009f4
#define SceneData_GetTablec138 Func_020009f8
#define SceneData_SelectDataByRuntimeSelector Func_02000a00
#define SceneData_SelectTableBySceneId Func_02003598

#include "select_overlay_data_by_runtime_selector.h"

extern u8 Data_0200bf70[];
extern u8 Data_0200c138[];
extern s16 Data_02000240[];
extern u8 Value_00000093;
extern u8 Value_00000094;
extern u8 Value_00000095;
extern u8 Value_00000096;
extern u8 Value_00000097;
extern u8 Data_0200c688[];
extern u8 Data_0200c724[];
extern u8 Data_0200c76c[];
extern u8 Data_0200c808[];
extern u8 Data_0200c850[];
extern u8 Data_0200c5e0[];

/* Contiguous unnamed leaf-owner run for resource_3bd. */

u8 *SceneData_GetTablebf70(void)
{
    return Data_0200bf70;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablec138(void)
{
    return Data_0200c138;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000093) {
        return (s32)Data_0200c688;
    }
    if (v == (s32)&Value_00000094) {
        return (s32)Data_0200c724;
    }
    if (v == (s32)&Value_00000095) {
        return (s32)Data_0200c76c;
    }
    if (v == (s32)&Value_00000096) {
        return (s32)Data_0200c808;
    }
    if (v == (s32)&Value_00000097) {
        return (s32)Data_0200c850;
    }
    return (s32)Data_0200c5e0;
}
