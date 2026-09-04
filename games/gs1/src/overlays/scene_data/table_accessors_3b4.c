/* Contiguous unnamed leaf-owner run for resource_3b4. */

#include "types.h"

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
#define SceneData_ReturnZero Func_02000a44
s32 SceneData_ReturnZero(void)
{
    return 0;
}

#include "types.h"

#define SceneData_GetTablea970 Func_02000a48

/* Complete eight-byte literal-address getter, including its sole pool word. */

u8 *SceneData_GetTablea970(void)
{
    return (u8 *)0x0200a970;
}
