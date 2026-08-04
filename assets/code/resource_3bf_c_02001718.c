#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_02003006(s32);
void Func_02006cd4(s32);

void Func_02001718(void)
{
    u8 *g = Data_03001ebc;

    if (Func_02003006(12) != 0 && Data_02000240[294] == 0) {
        s16 *q;
        s32 v;

        Func_02006cd4(0x02009719);
        q = (s16 *)(g + 386);
        v = 95;
        *q = v;
    }
}
