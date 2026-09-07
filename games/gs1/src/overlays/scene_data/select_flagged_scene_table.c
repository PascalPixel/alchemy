#include "types.h"

extern s32 Func_02001286(s32);
extern s32 Func_02001294(s32);
extern void Func_020012cc(void *);
extern u8 Data_020098b8[];
extern u8 Data_02009738[];
extern u8 Data_020095b8[];

void *SceneData_SelectFlaggedTable(void)
{
    void *tbl;

    if (Func_02001286(0x87a)) {
        tbl = Data_020098b8;
    } else if (Func_02001294(0x815)) {
        tbl = Data_02009738;
    } else {
        tbl = Data_020095b8;
    }
    Func_020012cc(tbl);
    return tbl;
}
