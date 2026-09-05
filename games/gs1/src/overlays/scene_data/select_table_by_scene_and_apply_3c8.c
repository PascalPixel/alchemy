#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000b5;
extern u8 Value_000000b6;
extern u8 Value_000000b7;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000ba;
extern u8 Data_0200e8ec[];
extern u8 Data_0200e904[];
extern u8 Data_0200e9c4[];
extern u8 Data_0200eb74[];
extern u8 Data_0200ec04[];
extern u8 Data_0200ec64[];
extern u8 Data_0200ecf4[];

extern void Func_02005d78(u8 *);

u8 *SceneData_SelectTableBySceneAndApply(void)
{
    u8 *ret;
    s16 *tbl;
    s16 v;

    tbl = Data_02000240;
    v = tbl[224];
    if (v == (s32) (u32) &Value_000000b5) {
        return Data_0200e904;
    }
    if (v == (s32) (u32) &Value_000000b6) {
        ret = Data_0200e9c4;
    } else if (v == (s32) (u32) &Value_000000b7) {
        ret = Data_0200eb74;
    } else if (v == (s32) (u32) &Value_000000b8) {
        ret = Data_0200ec04;
    } else if (v == (s32) (u32) &Value_000000b9) {
        ret = Data_0200ec64;
    } else if (v == (s32) (u32) &Value_000000ba) {
        ret = Data_0200ecf4;
    } else {
        goto no_match;
    }
    Func_02005d78(ret);
    return ret;

no_match:
    return Data_0200e8ec;
}
