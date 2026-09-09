#include "types.h"

#define SceneData_GetTableBaa8 Func_0200005c
#define SceneData_ReturnZero Func_02000064
#define SceneData_GetTableBbc8 Func_02000068
extern u8 Data_0200baa8;
extern u8 Data_0200bbc8;

void *SceneData_GetTableBaa8(void)
{
    return &Data_0200baa8;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTableBbc8(void)
{
    return &Data_0200bbc8;
}
