#include "types.h"

extern void Func_02005f50(s32);
extern void Func_02005f1c(s32, s32, s32, s32, s32, s32);
extern void Func_02005f18(s32, s32, s32, s32, s32, s32);
extern void Func_02005f28(s32, s32, s32, s32, s32, s32);
extern void Func_02005f54(s32, s32, s32, s32, s32, s32);
extern void Func_02005f4c(s32, s32, s32, s32, s32, s32);
extern void Func_02005f5c(s32, s32, s32, s32, s32, s32);

void Func_020010cc(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Func_02005f50(0x161);
    x = 23;
    y = 8;
    Func_02005f1c(36, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Func_02005f18(36, 8, 23, 8, b, a);
    Func_02005f28(100, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Func_02005f54(53, 55, 3, 3, x, y);
    Func_02005f4c(53, 55, 46, 55, a, a);
    Func_02005f5c(117, 55, 110, 55, a, a);
}
