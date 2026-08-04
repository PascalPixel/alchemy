#include "types.h"

extern s32 Func_020050fe(void);
extern void Func_020050d6(void);
extern void Func_0200516e(s32, s32);
extern void Func_020050dc(s32);
extern void Func_020051cc(s32, s32);
extern void Func_020050f8(void);

void Func_02000378(s32 x)
{
    u8 *flag = (u8 *)Func_020050fe() + 91;
    s32 zero = 0;

    *flag = 1;
    Func_020050d6();
    Func_0200516e(x, 1);
    Func_020050dc(2);
    Func_020051cc(x, 0);
    Func_020050f8();
    *flag = zero;
}
