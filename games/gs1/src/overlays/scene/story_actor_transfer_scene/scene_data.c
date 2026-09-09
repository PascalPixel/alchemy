#include "types.h"

#define SceneData_GetTableD27C Func_02000290
#define SceneData_ReturnZero Func_02000298
#define SceneData_GetTableDA2C Func_0200029c
#define SceneData_GetTableE3F4 Func_02000590
extern u8 Data_0200d27c[];
extern u8 Data_0200da2c[];
extern u8 Data_0200e3f4[];

u8 *SceneData_GetTableD27C(void)
{
    return Data_0200d27c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTableDA2C(void)
{
    return Data_0200da2c;
}

u8 *SceneData_GetTableE3F4(void)
{
    return Data_0200e3f4;
}
