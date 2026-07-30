typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_020054e0(s32 arg0);
extern struct S *Func_020054ea(s32 arg0);
extern struct S *Func_0200551e(s32 arg0);
extern void Func_02003be8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003bf8(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02005448(s32 arg0);
extern void Func_020054d4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_020054e4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02002450(void) {
    s32 x = Func_020054e0(17)->f08 >> 20;
    s32 y = Func_020054ea(17)->f10 >> 20;

    if (y == 15) {
        Func_02003be8(17, 0, 64);
    } else if (y == 18) {
        Func_02003bf8(17, 0, 16);
    } else if (y == 19) {
        return;
    }
    Func_02005448(2);
    {
        s32 k = Func_0200551e(17)->f10 >> 20;

        s32 m = x - 1;

        Func_020054d4(m, y, 3, 1, m, k);
    }
    Func_020054e4(0, 0, 3, 1, x - 1, y);
}
