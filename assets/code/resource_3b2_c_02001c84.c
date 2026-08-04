#include "types.h"

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02004d14(s32 arg0);
extern struct S *Func_02004d1e(s32 arg0);
extern struct S *Func_02004d28(s32 arg0);
extern struct S *Func_02004d84(s32 arg0);
extern void Func_0200342a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003436(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003440(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003454(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003464(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02004cae(s32 arg0);
extern void Func_02004d3a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_02004d4a(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02001c84(void) {
    s32 x = Func_02004d14(8)->f08 >> 20;
    s32 y = Func_02004d1e(8)->f10 >> 20;
    s32 z = Func_02004d28(12)->f08 >> 20;

    if (y == 7) {
        if (z == 24) {
            Func_0200342a(8, 0, 48);
        } else {
            Func_02003436(8, 0, 80);
            Func_02003440(8, 0, 112);
        }
    } else if (y == 10) {
        if (z == 24) {
            return;
        }
        Func_02003454(8, 0, 144);
    } else if (y == 14) {
        Func_02003464(8, 0, 80);
    } else {
        return;
    }
    Func_02004cae(2);
    {
        s32 k = Func_02004d84(8)->f10 >> 20;
        s32 m = x - 1;

        Func_02004d3a(m, y, 3, 1, m, k);
    }
    Func_02004d4a(0, 0, 3, 1, x - 1, y);
}
