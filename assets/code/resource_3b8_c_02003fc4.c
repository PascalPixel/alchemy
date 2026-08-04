#include "types.h"

extern u8 Value_000022a3;
extern void Func_02008466(s32);
extern s32 Func_02008476(s32, s32);
extern s32 Func_020083d6(s32, s32);
extern void Func_02008480(s32);
extern void Func_02008498(s32, s32);
extern void Func_02008490(s32);
extern void Func_020084a8(s32, s32);

void Func_02003fc4(s32 a)
{
    s32 k = (s32)&Value_000022a3;

    Func_02008466(k);
    Func_02008476(a, 0);
    if (Func_020083d6(0, 0) == 0) {
        Func_02008480(k + 1);
        Func_02008498(a, 0);
    } else {
        Func_02008490(k + 2);
        Func_020084a8(a, 0);
    }
}
