#include "types.h"

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02004e70(s32 arg0);
extern struct S *Func_02004e7a(s32 arg0);
extern struct S *Func_02004eb2(s32 arg0);
extern void Func_0200357c(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003588(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003592(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02004ddc(s32 arg0);
extern void Func_02004e68(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_02004e78(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02001de0(void) {
    s32 x = Func_02004e70(10)->f08 >> 20;
    s32 y = Func_02004e7a(10)->f10 >> 20;

    if (y != 18) {
        if (y == 10) {
            Func_0200357c(10, 0, 128);
        } else {
            Func_02003588(10, 0, 112);
            Func_02003592(10, 0, 64);
        }
        Func_02004ddc(2);
        {
            s32 k = Func_02004eb2(10)->f10 >> 20;
            s32 m = x - 1;

            Func_02004e68(m, y, 3, 1, m, k);
        }
        Func_02004e78(0, 0, 3, 1, x - 1, y);
    }
}
