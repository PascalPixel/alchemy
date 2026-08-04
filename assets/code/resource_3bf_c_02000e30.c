#include "types.h"

extern s32 Data_0200f754[];

void Func_0200641c(s32, s32, s32, s32, s32, s32);
void Func_0200642e(s32, s32, s32, s32, s32, s32);
void Func_02006442(s32, s32, s32, s32, s32, s32);

void Func_02000e30(s32 i)
{
    s32 a = Data_0200f754[i * 2];
    s32 b = Data_0200f754[i * 2 + 1];

    Func_0200641c(0x37, 0x79, 1, 3, a, b);
    Func_0200642e(0x38, 0x79, 1, 1, a + 1, b);
    Func_02006442(a, b - 0x3f, 1, 1, a, b - 0x3e);
}
