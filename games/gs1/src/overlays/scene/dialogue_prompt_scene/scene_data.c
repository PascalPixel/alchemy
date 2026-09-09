#include "types.h"

#define SceneData_SelectTableE1fcBySceneRangeOrFlag855 Func_02000310
#define SceneData_FindEntryAtPosition Func_02004704
extern s16 Data_02000240[];
extern u8 Data_0200e1fc[];
extern u8 Data_0200e250[];
extern u8 Data_0200de30[];
extern u8 Data_0200cf2c[];

s32 Func_02005042(s32);

u8 *SceneData_SelectTableE1fcBySceneRangeOrFlag855(void)
{
    s16 *tbl = Data_02000240;
    s32 v = tbl[225];
    s32 lo = 15;

    if (v <= 17) {
        if (v >= lo) {
            return Data_0200e1fc;
        }
    }
    if (Func_02005042(0x855) != 0) {
        return Data_0200e250;
    }
    return Data_0200de30;
}

u8 *SceneData_FindEntryAtPosition(s32 *o) {
    s32 x = (o[0] + (s32)0xFFC00000) >> 19;
    s32 y = (o[2] + (s32)0xFD900000) >> 19;
    u8 *e = Data_0200cf2c;
    u8 *ret = 0;
    u32 i;

    for (i = 0; i <= 36; i++, e += 16) {
        s32 a = e[0];

        if (a == x || a + 1 == x) {
            s32 b = e[1];

            if (b == y || b + 1 == y) {
                ret = e;
                break;
            }
        }
    }
    return ret;
}
