#include "types.h"

extern s32 *Func_02003fac(s32);
extern void Func_02003f56(s32, s32, s32, s32, s32, s32);
extern void Func_02003f6e(s32, s32, s32, s32, s32, s32);
extern void Func_02003f80(s32, s32, s32, s32, s32, s32);
extern void Func_02003f90(s32 *, s32);
extern void Func_02003f5c(void);
extern void Func_02003eea(s32);

void Func_0200040c(void)
{
    s32 *a = Func_02003fac(10);

    if (a != 0) {
        s32 x = 24;
        s32 y = 26;
        s32 t;

        Func_02003f56(x, 27, 2, 1, x, y);
        t = a[2] >> 20;
        if (t == 25) {
            Func_02003f6e(0, 0, 1, 1, t, y);
        } else {
            Func_02003f80(0, 0, 1, 1, x, y);
        }
        Func_02003f90(a, 0);
        ((u8 *)a)[0x55] = 0;
        Func_02003f5c();
        Func_02003eea(1);
    }
}
