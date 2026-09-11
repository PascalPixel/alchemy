#include "types.h"

#define SceneState5D Value_0000005d
#define SceneState5E Value_0000005e
#define SceneState5F Value_0000005f
#define SceneDataA234 Data_0200a234
#define SceneDataA2C4 Data_0200a2c4
#define SceneDataA39C Data_0200a39c
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000005d
#define SecondaryRuntimeSelector Value_0000005e
#define TertiaryRuntimeSelector Value_0000005f
#define PrimaryOverlayData Data_0200a420
#define SecondaryOverlayData Data_0200a450
#define TertiaryOverlayData Data_0200a624
#define DefaultOverlayData Data_0200a414
#define SelectSceneDataByState Func_02000d80
#define SceneData_SelectSecondaryByRuntimeSelector Func_0200169c

#include "select_overlay_data_by_runtime_selector.h"

extern s16 Data_02000240[];
extern u8 Value_0000005d;
extern u8 Value_0000005e;
extern u8 Value_0000005f;
extern u8 Data_0200a234[];
extern u8 Data_0200a2c4[];
extern u8 Data_0200a39c[];

s32 SelectSceneDataByState(void)
{
    s16 state = Data_02000240[224];

    if (state == (s32)&SceneState5D) {
        return (s32)SceneDataA234;
    }
    if (state == (s32)&SceneState5E) {
        return (s32)SceneDataA2C4;
    }
    if (state == (s32)&SceneState5F) {
        return (s32)SceneDataA39C;
    }
    return (s32)SceneDataA234;
}

s32 SceneData_SelectSecondaryByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}
