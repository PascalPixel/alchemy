#include "types.h"

extern void Func_02005b60(s32, s32, s32, s32, s32, s32);
extern void Func_02005b72(s32, s32, s32, s32, s32, s32);
extern void Func_02005b84(s32, s32, s32, s32, s32, s32);
extern void Func_02005b98(s32, s32, s32, s32, s32, s32);
extern void Func_02005baa(s32, s32, s32, s32, s32, s32);

void Func_02001498(void)
{
    s32 a = 26;
    s32 h = 0x47;
    s32 b;
    s32 a2;

    Func_02005b60(29, 20, 1, 1, a, h);
    b = 0x46;
    Func_02005b72(29, 20, 1, 1, a, b);
    a2 = 27;
    Func_02005b84(29, 20, 1, 1, a2, b);
    Func_02005b98(28, 21, 1, 1, 28, h);
    Func_02005baa(28, 22, 1, 1, a2, 0x48);
}
