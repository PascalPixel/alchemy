#include "types.h"

extern s32 Func_020041ba(s32, s32, s32, s32, s32, s32);
extern u8 *Func_02004228(s32);

void Func_02001ccc(void)
{
    s32 width = 42;
    s32 height = 55;
    u8 *entry;

    Func_020041ba(40, 54, 1, 1, width, height);
    entry = Func_02004228(10) + 85;
    *entry = 0;
}
