#include "types.h"

#define SceneData_GetTable9450 Func_02000498
#define SceneData_ReturnZero Func_020004a0
#define SceneData_GetTable95A0 Func_020004a4
#define SceneData_GetTable95D8 Func_020004ac
#define SceneData_GetTable97dc Func_02000f28
/* Complete table-address getter through its sole return and literal pool. */
extern const unsigned char Data_02009450[];
/* Complete overlay-local table-address getter through its literal pool. */
extern const unsigned char Data_020095a0[];
/* Complete overlay-local table-address getter through its literal pool. */
extern const unsigned char Data_020095d8[];
/* Complete in-image scene-descriptor getter through its literal pool. */
extern const u8 Data_020097dc[];

const void *SceneData_GetTable9450(void)
{
    return Data_02009450;
}

/* Complete zero-return leaf; no calls and no argument read. */
int SceneData_ReturnZero(void)
{
    return 0;
}

const void *SceneData_GetTable95A0(void)
{
    return Data_020095a0;
}

const void *SceneData_GetTable95D8(void)
{
    return Data_020095d8;
}

const u8 *SceneData_GetTable97dc(void)
{
    return Data_020097dc;
}
