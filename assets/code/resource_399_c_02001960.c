#include "types.h"

extern void Func_02003b80(s32);
extern void Func_02003c98(s32, s32);
extern void Func_02003b84(s32, s32, s32, s32, s32, s32);

void Func_02001960(void)
{
    s32 a;
    s32 b;

    Func_02003b80(0x203);
    Func_02003c98(11, 3);
    a = 15;
    b = 7;
    Func_02003b84(15, 6, 1, 1, a, b);
}
