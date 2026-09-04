/* Contiguous unnamed leaf-owner run for resource_3b8. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000008b;
extern u8 Data_0200ca1c[];
extern u8 Data_0200c614[];

#define SceneData_SelectTablec614ByState Func_02000030

s32 SceneData_SelectTablec614ByState(void) {
    if (Data_02000240[224] == (s32)&Value_0000008b) {
        return (s32)Data_0200ca1c;
    }
    return (s32)Data_0200c614;
}

#include "types.h"

extern u8 Data_0200ca7c[];

#define SceneData_GetTableca7c Func_02000060

s32 SceneData_GetTableca7c(void)
{
    return (s32)Data_0200ca7c;
}

#include "types.h"

#define SceneData_GetTableca8c Func_02000068

extern u8 Data_0200ca8c[];

s32 SceneData_GetTableca8c(void)
{
    return (s32)Data_0200ca8c;
}
