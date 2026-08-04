#include "types.h"

extern s32 Func_02003f5e(s32, s32, s32, s32, s32, s32);
extern u8 *Func_02003fcc(s32);

void Func_02001a70(void)
{
    s32 width = 41;
    s32 height = 42;
    u8 *entry;

    Func_02003f5e(44, 42, 1, 1, width, height);
    entry = Func_02003fcc(8) + 35;
    *entry = 2;
}
