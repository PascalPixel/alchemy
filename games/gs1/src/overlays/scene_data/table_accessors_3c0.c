/* Contiguous unnamed leaf-owner run for resource_3c0. */

#include "types.h"

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

#include "types.h"

/* Complete eight-byte literal-address getter, including its sole pool word. */

u8 *SceneData_GetTable95c0(void)
{
    return (u8 *)0x020095c0;
}
