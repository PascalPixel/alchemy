#include "types.h"

#define SceneData_SelectAndApplyTableBySceneId Func_020012e0

extern s16 Data_02000240[];
extern u8 Value_000000ac;
extern u8 Data_0200b8f4[];
extern u8 Data_0200ba74[];

extern void Func_02004402(u8 *);

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
