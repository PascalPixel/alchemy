#include "types.h"

#define Function Func_080a1fd4

s32 Func_080022ec();
s32 Func_080022fc();
void Func_0800352c();
void Func_08015418();
void Func_080f9010();

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

s32 Func_080a1fd4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4)
{
    s32 n;
    s32 v1;
    s32 v3;
    s32 v4;
    s32 v5;
    s32 w;
    s32 v2;
    s32 prod;

    if (a1 == 0) {
        return -1;
    }
    Call1(Func_08015418, 0x6002500);
    n = Value2(Func_080022ec, a1, a2);
    if (Value2(Func_080022fc, a1, a2) != 0) {
        n += 1;
    }
    if (a0 != 0) {
        v4 = (*(volatile s32 *)0x03001b04 & 16);
        v1 = (*(volatile s32 *)0x03001b04 & 32);
        v5 = (*(volatile s32 *)0x03001b04 & 64);
        w = (*(volatile s32 *)0x03001b04 & 128);
    } else {
        v4 = (*(volatile s32 *)0x03001b04 & 128);
        v1 = (*(volatile s32 *)0x03001b04 & 64);
        v5 = (*(volatile s32 *)0x03001b04 & 32);
        w = (*(volatile s32 *)0x03001b04 & 16);
    }
    if (v5 != 0) {
        Call1(Func_080f9010, 111);
        v3 = (*(s32 *)a4 - 1);
        *(s32 *)a4 = v3;
        if (v3 < 0) {
            *(s32 *)a4 = (n - 1);
        }
        prod = a2 * *(s32 *)a4;
        if ((*(s32 *)a3 + prod) <= (a1 - 1)) {
            goto L_done;
        }
    } else {
        if (w == 0) {
            goto L_other;
        }
        Call1(Func_080f9010, 111);
        v3 = (*(s32 *)a4 + 1);
        *(s32 *)a4 = v3;
        if (v3 > (n - 1)) {
            *(s32 *)a4 = 0;
        }
        prod = a2 * *(s32 *)a4;
        if ((*(s32 *)a3 + prod) <= (a1 - 1)) {
            goto L_done;
        }
    }
    v3 = (a1 - prod) - 1;
    *(s32 *)a3 = v3;
    if (v3 > (a2 - 1)) {
        *(s32 *)a3 = (a2 - 1);
    }
L_done:
    Func_0800352c();
    return 1;
L_other:
    if (v1 != 0) {
        Call1(Func_080f9010, 111);
        v3 = (*(s32 *)a3 - 1);
        *(s32 *)a3 = v3;
        if (v3 >= 0) {
            return 0;
        }
        v3 = ((a1 - (s32)(*(s32 *)a4 * a2)) - 1);
        *(s32 *)a3 = v3;
        if (v3 <= (a2 - 1)) {
            return 0;
        }
        *(s32 *)a3 = (a2 - 1);
    } else {
        if (v4 == 0) {
            return -1;
        }
        Call1(Func_080f9010, 111);
        v2 = (*(s32 *)a3 + 1);
        *(s32 *)a3 = v2;
        if (v2 == (a1 - (s32)(*(s32 *)a4 * a2))) {
            *(s32 *)a3 = 0;
        }
        if (*(s32 *)a3 > (a2 - 1)) {
            *(s32 *)a3 = 0;
        }
    }
    return 0;
}
