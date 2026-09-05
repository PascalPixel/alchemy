/* Contiguous unnamed leaf-owner run for resource_3ad. */

#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_0000006a;
extern u8 Data_02009cd8[];
extern u8 Data_02009cc0[];

#define SceneData_SelectTable9cc0ByState Func_02000044

s32 SceneData_SelectTable9cc0ByState(void) {
    if (Data_02000240[224] == (s32)&Value_0000006a) {
        return (s32)Data_02009cd8;
    }
    return (s32)Data_02009cc0;
}

#include "types.h"

#define SceneData_GetTable9dd4 Func_02000074

u8 *SceneData_GetTable9dd4(void)
{
    return (u8 *)0x02009dd4;
}
