#include "types.h"

extern u8 *Data_03001ee0;

extern s32 Func_02003a6e(s32);

void Func_0200170c(void)
{
    s32 *flag = (s32 *)0x0200B328;

    if (*flag != 0) {
        u8 *state = Data_03001ee0;

        *(s32 *)(state + 24) = Func_02003a6e(0);
    }
}
