#include "types.h"

#define SceneData_GetTablee3d4 Func_02000464
#define SceneData_ReturnZero Func_0200046c
#define SceneData_GetTablee464 Func_02000470
#define SceneData_GetTablee478 Func_02000478
#define SceneData_GetTableE6ec Func_02000714
extern unsigned char Data_0200e3d4[];
extern unsigned char Data_0200e464[];
extern unsigned char Data_0200e478[];
extern unsigned char Data_0200e6ec[];

/* Contiguous unnamed leaf-owner run for resource_3c9. */

s32 SceneData_GetTablee3d4(void)
{
    return (s32)Data_0200e3d4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTablee464(void)
{
    return (s32)Data_0200e464;
}

s32 SceneData_GetTablee478(void)
{
    return (s32)Data_0200e478;
}

s32 SceneData_GetTableE6ec(void)
{
    return (s32)Data_0200e6ec;
}
