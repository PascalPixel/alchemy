#include "types.h"

extern void Func_02005ed4(s32);
extern void Func_02005e98(s32, s32, s32, s32, s32, s32);
extern void Func_02005e94(s32, s32, s32, s32, s32, s32);
extern void Func_02005ea4(s32, s32, s32, s32, s32, s32);
extern void Func_02005ed0(s32, s32, s32, s32, s32, s32);
extern void Func_02005ec8(s32, s32, s32, s32, s32, s32);
extern void Func_02005ed8(s32, s32, s32, s32, s32, s32);

void Func_02001048(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Func_02005ed4(0x161);
    x = 23;
    y = 8;
    Func_02005e98(35, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Func_02005e94(35, 8, 23, 8, b, a);
    Func_02005ea4(99, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Func_02005ed0(57, 55, 3, 3, x, y);
    Func_02005ec8(57, 55, 46, 55, a, a);
    Func_02005ed8(121, 55, 110, 55, a, a);
}
