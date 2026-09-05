#include "types.h"

extern s16 Data_02000240[];
extern u8 Data_0200e1fc[];
extern u8 Data_0200e250[];
extern u8 Data_0200de30[];
extern s32 Func_02005042(s32);

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
