#include "types.h"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_00000036
#define SecondaryRuntimeSelector Value_00000037
#define TertiaryRuntimeSelector Value_00000038
#define PrimaryOverlayData Data_0200ead8
#define SecondaryOverlayData Data_0200ec10
#define TertiaryOverlayData Data_0200ed60
#define DefaultOverlayData Data_0200eec8
#define SceneData_SelectDataByRuntimeSelector Func_02000f30
#define SceneData_LoadBlockA2c5 Func_020022ec
#define SceneData_ApplyTableA2c5AndReturnZero Func_02002304

#include "select_overlay_data_by_runtime_selector.h"

extern u8 Data_0200a2c5[];

void Func_02007c7a(s32, s32);
void Func_02007c96(s32);

s32 SceneData_SelectDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

s32 SceneData_LoadBlockA2c5(void)
{
    s32 n = 0xc80;

    Func_02007c7a((s32)Data_0200a2c5, n);
    return 0;
}

s32 SceneData_ApplyTableA2c5AndReturnZero(void)
{
    Func_02007c96((s32)Data_0200a2c5);
    return 0;
}
