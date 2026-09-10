#include "types.h"

#define SceneData_GetTable9c34 Func_02000030
#define SceneData_GetTable9CAC Func_0200003c
#define SceneData_SelectTable9cc0ByState Func_02000044
#define SceneData_GetTable9dd4 Func_02000074
#define SceneData_ReturnOne Func_0200131c
extern s16 Data_02000240[];
extern u8 Value_0000006a;
extern u8 Data_02009cd8[];
extern u8 Data_02009cc0[];

/* Contiguous unnamed leaf-owner run for resource_3ad. */

/*
 * Overlay resource_3ad. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x02000030 includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 * Another overlay exports the same address, but only one overlay is resident
 * at 0x02000000 at a time, so that is a different table -- do not merge the
 * two getters.
 */
u8 *SceneData_GetTable9c34(void)
{
    return (u8 *)0x02009c34;
}

/*
 * Overlay resource_3ad. Exported getter for an in-image table, published
 * from the overlay header.
 */

/*
 * The eight-byte owner at 0x0200003c includes its one pool word, which holds
 * the returned address; the table is returned without being dereferenced.
 */
u8 *SceneData_GetTable9CAC(void)
{
    return (u8 *)0x02009cac;
}

s32 SceneData_SelectTable9cc0ByState(void)
{
    if (Data_02000240[224] == (s32)&Value_0000006a) {
        return (s32)Data_02009cd8;
    }
    return (s32)Data_02009cc0;
}

u8 *SceneData_GetTable9dd4(void)
{
    return (u8 *)0x02009dd4;
}

s32 SceneData_ReturnOne(void)
{
    return 1;
}
