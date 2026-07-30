typedef signed int s32;

struct S {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
};

extern struct S *Func_02005008(s32 arg0);
extern struct S *Func_02005012(s32 arg0);
extern struct S *Func_0200505c(s32 arg0);
extern void Func_02003714(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0200372a(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02003736(s32 arg0, s32 arg1, s32 arg2);
extern void Func_02004f86(s32 arg0);
extern void Func_02005012_b(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern void Func_02005022(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);

void Func_02001f78(void) {
    s32 x = Func_02005008(12)->f08 >> 20;
    s32 y = Func_02005012(12)->f10 >> 20;

    if (x == 36) {
        Func_02003714(12, -96, 0);
        Func_02003736(12, -96, 0);
    } else if (x == 34) {
        Func_0200372a(12, -96, 0);
        Func_02003736(12, -64, 0);
    } else if (x == 24) {
        return;
    }
    Func_02004f86(2);
    {
        s32 k = Func_0200505c(12)->f08 >> 20;
        s32 m = y - 1;

        Func_02005012_b(x, m, 1, 3, k, m);
    }
    Func_02005022(0, 0, 1, 3, x, y - 1);
}
