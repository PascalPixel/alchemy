#include "types.h"

#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000001e
#define SecondaryRuntimeSelector Value_00000023
#define TertiaryRuntimeSelector Value_00000020
#define PrimaryOverlayData Data_020091ec
#define SecondaryOverlayData Data_0200930c
#define TertiaryOverlayData Data_0200936c
#define DefaultOverlayData Data_020091d4
#define SceneData_SelectOverlayDataByRuntimeSelector Func_02000088
#define SceneData_GetTable93FCWhenState20 Func_020000dc
#define SceneData_GetTable942c Func_02000104

#include "select_overlay_data_by_runtime_selector.h"

extern s16 Data_02000240[];
extern u8 Value_00000020;
extern u8 Data_020093fc[];

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
#include "select_overlay_data_by_runtime_selector_body.inc"
}

s32 SceneData_GetTable93FCWhenState20(void)
{
    if (Data_02000240[224] == (s32)&Value_00000020) {
        return (s32)Data_020093fc;
    }
    return 0;
}

/*
 * Table getter for resource_38b. The owner at 0x02000104 is eight bytes and
 * includes its one pool word at 0x02000108: the pc-relative load reads that
 * word, so the word belongs to this owner. The word is an address returned
 * without being dereferenced. Many getters share this body, but each returns
 * a different address.
 */
u8 *SceneData_GetTable942c(void)
{
    return (u8 *)0x0200942c;
}
