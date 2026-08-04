#include "types.h"

extern void Func_02005d68(s32, s32, s32, s32, s32, s32);
extern void Func_02005d7a(s32, s32, s32, s32, s32, s32);
extern void Func_02005d8e(s32, s32, s32, s32, s32, s32);
extern void Func_02005da0(s32, s32, s32, s32, s32, s32);
extern void Func_02005d94(void);

void Func_020028dc(s32 a)
{
    if (a != 0) {
        s32 x;
        x = 2;
        Func_02005d68(9, 45, 65, 5, x, x);
        Func_02005d7a(11, 46, 67, 6, 1, x);
    } else {
        s32 x;
        x = 2;
        Func_02005d8e(89, 2, 65, 5, x, x);
        Func_02005da0(102, 32, 67, 6, 1, x);
    }
    Func_02005d94();
}
