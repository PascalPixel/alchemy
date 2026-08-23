#include "types.h"

s32 Func_0800430c(void);
s32 Func_08009290(void);
s32 GameFlag_Set(s32 flag_no);

s32 Func_080a1070(void)
{
    GameFlag_Set(0x152);
    GameFlag_Set(0x166);
    Func_08009290();
    return Func_0800430c();
}
