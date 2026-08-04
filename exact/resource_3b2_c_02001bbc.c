#include "types.h"

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02004c4c(s32 arg0);
extern struct S *Func_02004c56(s32 arg0);
extern struct S *Func_02004c60(s32 arg0);
extern struct S *Func_02004c6a(s32 arg0);
extern struct S *Func_02004cda(s32 arg0);
extern void Func_02003376(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003386(s32 arg0, s32 arg1, s32 arg2);
extern void Func_020033b2(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003758(void);
extern void Func_02004c04(s32 arg0);
extern void Func_02004c90(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_02004ca0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02001bbc(void) {
    s32 x = Func_02004c4c(8)->f08 >> 20;
    s32 y = Func_02004c56(8)->f10 >> 20;
    s32 z = Func_02004c60(12)->f08 >> 20;
    s32 w = Func_02004c6a(15)->f08 >> 20;

    if (y == 19) {
        if (z == 24) {
            Func_020033b2(8, 0, -80);
        } else if (w == 24) {
            Func_02003376(8, 0, -112);
            Func_020033b2(8, 0, -32);
        } else {
            Func_02003386(8, 0, -80);
            Func_020033b2(8, 0, -112);
        }
    } else if (y == 14) {
        if (z == 24) {
            return;
        }
        if (w == 24) {
            Func_020033b2(8, 0, -64);
        } else {
            Func_020033b2(8, 0, -112);
        }
    } else if (y == 10) {
        if (w == 24) {
            return;
        }
        Func_020033b2(8, 0, -48);
    } else {
        Func_02003758();
        return;
    }
    Func_02004c04(2);
    {
        s32 k = Func_02004cda(8)->f10 >> 20;
        s32 m = x - 1;

        Func_02004c90(m, y, 3, 1, m, k);
    }
    Func_02004ca0(0, 0, 3, 1, x - 1, y);
}
