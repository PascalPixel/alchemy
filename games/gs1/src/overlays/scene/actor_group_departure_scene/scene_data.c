#include "types.h"

#define SceneData_GetTableD0E4 Func_020000e8
#define SceneData_ReturnZero Func_020000f0
#define SceneData_GetTableD27c Func_020000f4
#define SceneData_GetTableD2B8 Func_020000fc
#define SceneData_GetTableD558 Func_02000104
extern u8 Data_0200d0e4[];
extern u8 Data_0200d27c[];
extern u8 Data_0200d2b8[];
extern u8 Data_0200d558[];

u8 *SceneData_GetTableD0E4(void)
{
    return Data_0200d0e4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableD27c(void)
{
    return Data_0200d27c;
}

u8 *SceneData_GetTableD2B8(void)
{
    return Data_0200d2b8;
}

u8 *SceneData_GetTableD558(void)
{
    return Data_0200d558;
}
