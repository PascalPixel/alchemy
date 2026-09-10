#include "types.h"

#define SceneData_SelectDataByRuntimeSelector Func_02001270
#define SceneData_SelectTableB81cByWord224 Func_020012b0
#define SceneData_GetTableB85c Func_020012d8
#define SceneData_SelectAndApplyTableBySceneId Func_020012e0
#define SceneData_SelectTableByWord224 Func_02002370
extern s16 Data_02000240[];
extern u8 Value_000000ac;
extern u8 Value_000000ad;
extern u8 Data_0200b474[];
extern u8 Data_0200b654[];
extern u8 Data_0200b42c[];
extern u8 Data_0200b81c[];
extern u8 Data_0200b8f4[];
extern u8 Data_0200ba74[];
extern u8 Data_0200bc0c[];
extern u8 Data_0200bef4[];

void Func_02004402(u8 *);

/* Return this overlay's state block. */

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000ac) {
        return (s32)Data_0200b474;
    }
    if (v == (s32)&Value_000000ad) {
        return (s32)Data_0200b654;
    }
    return (s32)Data_0200b42c;
}

s32 SceneData_SelectTableB81cByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_000000ad) {
        return (s32)Data_0200b81c;
    }
    return 0;
}

u8 *SceneData_GetTableB85c(void) { return (u8 *)0x0200b85c; }

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
    u8 *tbl;

    if (Data_02000240[224] == (s32)&Value_000000ac) {
        tbl = Data_0200b8f4;
    } else {
        tbl = Data_0200ba74;
    }
    Func_02004402(tbl);
    return tbl;
}

s32 SceneData_SelectTableByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_000000ac) {
        return (s32)Data_0200bc0c;
    }
    return (s32)Data_0200bef4;
}
