#include "types.h"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000039
#define SecondaryRuntimeSelector Value_00000038
#define TertiaryRuntimeSelector Value_00000037
#define PrimaryOverlayData Data_0200e50c
#define SecondaryOverlayData Data_0200e35c
#define TertiaryOverlayData Data_0200e23c
#define DefaultOverlayData Data_0200e0ec
#define SceneData_SelectOverlayDataByRuntimeSelector Func_02000e20
#define SceneData_ReturnZero Func_02000e74
#define SceneData_GetTableE614 Func_02000e78
#define SceneData_SelectTableByWord224 Func_02000e80

#include "select_overlay_data_by_runtime_selector.h"

extern s16 Data_02000240[];
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000038;
extern u8 Value_00000039;
extern u8 Data_0200e700[];
extern u8 Data_0200e7a8[];
extern u8 Data_0200e838[];
extern u8 Data_0200e988[];
extern u8 Data_0200e6e8[];

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableE614(void)
{
    return (u8 *)0x0200e614;
}

s32 SceneData_SelectTableByWord224(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000036) {
        return (s32)Data_0200e700;
    }
    if (v == (s32)&Value_00000037) {
        return (s32)Data_0200e7a8;
    }
    if (v == (s32)&Value_00000038) {
        return (s32)Data_0200e838;
    }
    if (v == (s32)&Value_00000039) {
        return (s32)Data_0200e988;
    }
    return (s32)Data_0200e6e8;
}
