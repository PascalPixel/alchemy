#include "types.h"

extern void Func_02005cd8(s32, s32, s32, s32, s32, s32);
extern void Func_02005cea(s32, s32, s32, s32, s32, s32);
extern void Func_02005cfa(s32, s32, s32, s32, s32, s32);
extern void Func_02005d0e(s32, s32, s32, s32, s32, s32);
extern void Func_02005d1e(s32, s32, s32, s32, s32, s32);
extern void Func_02005d30(s32, s32, s32, s32, s32, s32);
extern void Func_02005d40(s32, s32, s32, s32, s32, s32);
extern void Func_02005d34(void);

void Func_0200284c(s32 a)
{
    if (a != 0) {
        s32 x;
        s32 y;
        x = 1;
        Func_02005cd8(8, 47, 64, 7, x, x);
        y = 2;
        Func_02005cea(7, 48, 63, 8, y, x);
        Func_02005cfa(7, 49, 63, 9, y, x);
    } else {
        s32 x;
        x = 1;
        Func_02005d0e(56, 0, 64, 7, x, x);
        Func_02005d1e(56, 0, 63, 8, x, x);
        Func_02005d30(56, 0, 63, 9, 2, x);
        Func_02005d40(58, 25, 64, 8, x, x);
    }
    Func_02005d34();
}
