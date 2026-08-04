#include "types.h"

s32 Func_0200326a(void);
s32 Func_02003114(s32);
s32 Func_02003186(s32);

s32 Func_020018e4(s32 arg0)
{
    if (Func_0200326a() == 0) {
        return 0;
    }
    if (Func_02003114(arg0) != 0) {
        return 1;
    }
    {
        s32 r = Func_02003186(arg0);

        /* branchless "r != 0" */
        return (u32)(r | -r) >> 31;
    }
}
