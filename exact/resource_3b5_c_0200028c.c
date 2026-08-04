#include "types.h"

extern void Func_02000fbe(s32, s32, s32, s32, s32, s32);
extern void Func_02001104(s32, s32, s32);

void Func_0200028c(void)
{
    s32 width = 3;
    s32 height = 26;

    Func_02000fbe(2, 25, 1, 1, width, height);
    Func_02001104(102, -1, -1);
}
