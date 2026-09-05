/* Contiguous unnamed leaf-owner run for resource_376. */

/* Complete eight-byte in-image address getter, including its pool word. */
void *SceneData_GetTable9478(void)
{
    return (void *)0x02009478;
}

/* Complete four-byte zero leaf. */
int SceneData_ReturnZero(void)
{
    return 0;
}

#include "types.h"

extern s32 Func_02001262(s32 flag);

/* Complete 36-byte flag-selected table getter through its three-word pool. */
void *SceneData_SelectTable9568ByFlag(void)
{
    if (Func_02001262(0x834) != 0)
        return (void *)0x02009590;
    return (void *)0x02009568;
}
