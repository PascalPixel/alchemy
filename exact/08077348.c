#include "types.h"

extern u8 Data_02000240[];
s32 Func_080795fc(void);
u8 *Func_08077394(s32);
u32 Func_080022ec(s32, s32);
u32 Func_08077348(void)
{
    s32 count;
    s32 total;
    s32 i;

    total = 0;
    count = Func_080795fc();
    if (count == 0) {
        return 0;
    }
    for (i = 0; i < count; i++) {
        total += Func_08077394(Data_02000240[0x1F8 + i])[15];
    }
    total = Func_080022ec(total, count);
    return total;
}
