#include "types.h"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_000000b0
#define SecondaryRuntimeSelector Value_000000af
#define TertiaryRuntimeSelector Value_000000ae
#define PrimaryOverlayData Data_0200b270
#define SecondaryOverlayData Data_0200b330
#define TertiaryOverlayData Data_0200b4f8
#define DefaultOverlayData Data_0200b558
#define SceneData_SelectByRuntimeSelector Func_02000f58
#define SceneData_SelectTableB5b8ByState Func_02000fac
#define SceneData_GetTableb5f8 Func_02000fd4

#include "select_overlay_data_by_runtime_selector.h"

extern s16 Data_02000240[];
extern u8 Value_000000b0;
extern u8 Data_0200b5b8[];

/* Contiguous unnamed leaf-owner run for resource_3c5. */

/* Return this overlay's state block. */

s32 SceneData_SelectByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

s32 SceneData_SelectTableB5b8ByState(void)
{
    if (Data_02000240[224] == (s32)&Value_000000b0) {
        return (s32)Data_0200b5b8;
    }
    return 0;
}

u8 *SceneData_GetTableb5f8(void) { return (u8 *)0x0200b5f8; }
