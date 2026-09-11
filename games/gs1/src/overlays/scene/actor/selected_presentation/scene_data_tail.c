#include "types.h"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_000000a4
#define SecondaryRuntimeSelector Value_000000a5
#define TertiaryRuntimeSelector Value_000000a6
#define PrimaryOverlayData Data_02009610
#define SecondaryOverlayData Data_020096b8
#define TertiaryOverlayData Data_02009790
#define DefaultOverlayData Data_020095f8
#define SceneData_SelectOverlayDataByRuntimeSelector Func_020003ac
#define SceneData_SelectTable99C4Or97B4ByStateA6 Func_02000e5c

#include "select_overlay_data_by_runtime_selector.h"

extern s16 Data_02000240[];
extern u8 Value_000000a6;
extern u8 Data_020099c4[];
extern u8 Data_020097b4[];

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

s32 SceneData_SelectTable99C4Or97B4ByStateA6(void)
{
    if (Data_02000240[224] == (s32)&Value_000000a6) {
        return (s32)Data_020099c4;
    }
    return (s32)Data_020097b4;
}
