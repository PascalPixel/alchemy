#include "types.h"

extern s32 Func_02000dc4(s32, s32);
extern s32 Func_02000dd2(s32, s32);
extern s32 Func_02000de2(s32, s32);
extern s32 Func_02000df2(s32, s32);

s32 Func_02000714(s32 x, s32 y)
{
    if (Func_02000dc4(x, y - 0x180000) != 0
     || Func_02000dd2(x, y - 0x80000) != 0
     || Func_02000de2(x, y + 0x80000) != 0
     || Func_02000df2(x, y + 0x180000) != 0) {
        return -1;
    }
    return 0;
}
