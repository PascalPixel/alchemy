#include "types.h"

#define SceneData_SelectTablec614ByState Func_02000030
#define SceneData_GetTableca7c Func_02000060
#define SceneData_GetTableca8c Func_02000068
#define SceneData_SelectDataBySelectorAndFlags Func_02000070
extern u8 Value_0000008b;
extern u8 Data_0200ca1c[];
extern u8 Data_0200c614[];
extern u8 Data_0200ca7c[];
extern u8 Data_0200ca8c[];
extern u8 Data_0200cb3c[];
extern u8 Data_0200ce6c[];
extern u8 Data_0200cd64[];
extern u8 Data_0200cb84[];

s32 Func_0200442e(s32);
s32 Func_0200443c(s32);

/* Contiguous unnamed leaf-owner run for resource_3b8. */

s32 SceneData_SelectTablec614ByState(void)
{
    extern s16 Data_02000240[];

    if (Data_02000240[224] == (s32)&Value_0000008b) {
        return (s32)Data_0200ca1c;
    }
    return (s32)Data_0200c614;
}

s32 SceneData_GetTableca7c(void)
{
    extern s16 Data_02000240[];

    return (s32)Data_0200ca7c;
}

s32 SceneData_GetTableca8c(void)
{
    extern s16 Data_02000240[];

    return (s32)Data_0200ca8c;
}

s32 SceneData_SelectDataBySelectorAndFlags(void)
{
    extern u8 Data_02000240[];

    s16 *tbl = (s16 *)Data_02000240;

    if (tbl[0xe0] == (s32)&Value_0000008b)
        return (s32)Data_0200cb3c;
    if (Func_0200442e(0x950) != 0)
        return (s32)Data_0200ce6c;
    if (Func_0200443c(0x962) != 0)
        return (s32)Data_0200cd64;
    return (s32)Data_0200cb84;
}
