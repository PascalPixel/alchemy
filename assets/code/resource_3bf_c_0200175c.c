#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_02006d76(s32);
s32 Func_02003054(s32);
void Func_02006da2(s32);
void Func_02006d28(s32);
void Func_02006d2e(s32);

void Func_0200175c(void)
{
    u8 *g = Data_03001ebc;

    if (Func_02006d76(549) == 0 && Func_02003054(13) != 0
        && Data_02000240[294] == 0) {
        s16 *q;
        s32 v;

        Func_02006da2(549);
        Func_02006d28(0x0200975d);
        Func_02006d2e(0x020097bd);
        q = (s16 *)(g + 386);
        v = 96;
        *q = v;
    }
}
