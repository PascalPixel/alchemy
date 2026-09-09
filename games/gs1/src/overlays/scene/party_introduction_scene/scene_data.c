#include "types.h"

#define SceneData_GetTableCd88 Func_0200036c
#define SceneData_ReturnZero Func_02000374
#define SceneData_GetTableCDB8 Func_02000378
#define SceneData_GetTableCdc4 Func_02000380
#define SceneData_GetTableCFA4 Func_02000388
extern s32 Data_0200cd88[];
extern s32 Data_0200cdb8[];
extern s32 Data_0200cdc4[];
extern s32 Data_0200cfa4[];

s32 *SceneData_GetTableCd88(void)
{
    return Data_0200cd88;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 *SceneData_GetTableCDB8(void)
{
    return Data_0200cdb8;
}

s32 *SceneData_GetTableCdc4(void)
{
    return Data_0200cdc4;
}

s32 *SceneData_GetTableCFA4(void)
{
    return Data_0200cfa4;
}
