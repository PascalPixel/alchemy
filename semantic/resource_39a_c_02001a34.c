#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

extern void Func_02001ad0(s32, s32);
extern s32 Func_080770c0(s32);

void Func_02001a34(void)
{
    s32 scene = *(s16 *)(Data_03001ebc + 0x16c);
    s32 state = Data_02000240[224];

    if (state == 0x3f) {
        if (scene == 17)
            Func_02001ad0(0, -32);
        else
            Func_02001ad0(-32, 0);
    }

    if (Data_02000240[224] == 0x40 && scene == 25 &&
        Func_080770c0(0x309) != 0)
        Func_02001ad0(0, 32);
}
